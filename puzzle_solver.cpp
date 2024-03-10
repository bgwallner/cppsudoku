#include "puzzle_solver.h"

#include <tuple>
#include <iostream>

#include "constants.h"

namespace puzzle_solver
{

inline namespace
{

int GetGroupNbr(const int row, const int col)
{
    int grp_row = row / 3;
    int grp_col = col / 3;
    return grp_row * 3 + grp_col;
}

std::tuple<int, int> GetRowColFromGrp(const int grp_nbr)
{
    int start_row = (grp_nbr / 3) * 3;
    int start_col = (grp_nbr % 3) * 3;
    return std::make_tuple(start_row, start_col);
}

} // namespace

int PuzzleSolver::GetFirstFreeElement(const std::vector<std::vector<int>>& puzzle,
	int& row, int& col)
{
    // Find first element having value = 0. If none found puzzle is solved.
    for (int row_{ 0 }; row_ < kDim; row_++)
    {
        for (int col_{ 0 }; col_ < kDim; col_++)
        {
            if (0 == puzzle[row_][col_])
            {
                /* Found available element */
                row = row_;
                col = col_;
                return kOK;
            }
        }
    }
    return kNotOK;
}

void PuzzleSolver::InitializeStats(const std::vector<std::vector<int>>& puzzle)
{
    // Initialize for row and grp
    for (int row{ 0 }; row < kDim; row++)
    {
        for (int col{ 0 }; col < kDim; col++)
        {
            if (puzzle[row][col] != 0)
            {
                AddToRowSum(row, 1);
                AddToGrpSum(row, col, 1);
            }
        }
    }

    // Initialize for col
    for (int col{ 0 }; col < kDim; col++)
    {
        for (int row{ 0 }; row < kDim; row++)
        {
            if (puzzle[row][col] != 0)
            {
                AddToColSum(col, 1);
            }
        }
    }
}

int PuzzleSolver::GetGroupSum(const int row, const int col)
{
    return stats.grp_sums[GetGroupNbr(row, col)];
}

void PuzzleSolver::AddToGrpSum(const int row, const int col, const int value)
{
    stats.grp_sums[GetGroupNbr(row, col)] += value;
}

int PuzzleSolver::GetElementMRV(const std::vector<std::vector<int>>& puzzle,
	int& row, int& col)
{
    int score{0}, highest_score{0}, status{kNotOK};

    // Find the most constrained zero-element.
    // If none found puzzle is solved.
    for (int row_{ 0 }; row_ < kDim; row_++)
    {
        for (int col_{ 0 }; col_ < kDim; col_++)
        {
            if (0 == puzzle[row_][col_])
            {
                // Calculate score for row
                score = GetRowSum(row_);
                // Calculate score for col
                score += GetColSum(col_);
                // Calculate score for group
                score += GetGroupSum(row_, col_);

                // Check against highest score
                if(score > highest_score)
                {
                    highest_score = score;
                    row = row_;
                    col = col_;
                }

                // Status is kOK until no element with
                // value = 0 can be found
                status = kOK;
            }
        }
    }
    return status;
}

std::vector<std::tuple<int,int>> PuzzleSolver::CompleteEigthElemGroups(std::vector<std::vector<int>>& puzzle)
{
    // Data structure for keeping track of assigned zero index
    std::vector<std::tuple<int,int>> zero_elems{};
    // Check if any group has 8 elements and in case add them to a vector
    std::vector<int> indexes;
    for (int i{ 0 }; i < stats.grp_sums.size(); i++)
    {
        if (stats.grp_sums[i] == 8) {
            indexes.push_back(i);
        }
    }

    // Get the start row, col from group
    for (int i{ 0 }; i < indexes.size(); i++)
    {
        std::tuple<int,int> row_col = GetRowColFromGrp(indexes[i]);
        
        int row = std::get<0>(row_col);
        int col = std::get<1>(row_col);

        // Get the missing number (total grp sum = 45)
        int nbr = 45 - (puzzle[row][col] + puzzle[row][col + 1] + puzzle[row][col + 2] +
            puzzle[row + 1][col] + puzzle[row + 1][col + 1] + puzzle[row + 1][col + 2] +
            puzzle[row + 2][col] + puzzle[row + 2][col + 1] + puzzle[row + 2][col + 2]);

        // Get the row, col where nbr is zero
        int row_{ 0 };
        int col_{ 0 };
        std::tuple<int,int> temp {0,0};
        for (row_=row; row_ <= (row + 2); row_++)
        {
            for (col_=col; col_ <= (col + 2); col_++)
            {
                if (puzzle[row_][col_] == 0)
                {
                    puzzle[row_][col_] = nbr;

                    // Update stats data structure
                    AddToRowSum(row_, 1);
                    AddToColSum(col_, 1);
                    AddToGrpSum(row_, col_, 1);

                    // Save row_ and col_ for later (if we need to revert)
                    temp = {row_, col_};
                    zero_elems.push_back(temp);
                }
            }
        }
    }
    return zero_elems;
}

void PuzzleSolver::RevertEightElemGroupAssign(std::vector<std::vector<int>>& puzzle,
const std::vector<std::tuple<int,int>>& indexes)
{
    for(int i{0}; i<indexes.size(); i++)
    {
        int row = std::get<0>(indexes[i]);
        int col = std::get<1>(indexes[i]);
        puzzle[row][col] = 0;

        // Remove value from row_sum, col_sum and grp_sum.
        AddToRowSum(row, -1);
        AddToColSum(col, -1);
        AddToGrpSum(row, col, -1);
    }
}

PuzzleSolver::PuzzleSolver() {};

PuzzleSolver::~PuzzleSolver() {};

int PuzzleSolver::ForwardSolver(std::vector<std::vector<int>>& puzzle)
{
    int row{ 0 }, col{ 0 };

    // Increase counter for every recursion
    recursion_counter++;
 
    // Find first element with value 0
    if (kOK == GetFirstFreeElement(puzzle, row, col)) // <--- here iteration in row,col takes place
    {
        // Test all values from 1..kMaxVal
        for (int value{ 1 }; value <= kMaxVal; value++)
        {
            // Check if uiValue is valid for (row, col)
            if ((kOK == checker.IsInColValid(value, puzzle, row, col)) &&
                (kOK == checker.IsInRowValid(value, puzzle, row, col)) &&
                (kOK == checker.IsInGroupValid(value, puzzle, row, col)))
            {
                // Assign possible candidate for recursion N. */
                puzzle[row][col] = value;

                // Try to solve for recursion N+1 with the 'new' puzzle
                if (kOK == ForwardSolver(puzzle))
                {
                    // Puzzle solved (will generate an avalange of kOK).
                    // We will not end up here until it has been detected
                    // that no zero-elements can be found.
                    return kOK;
                }
            }
        }
        // Puzzle could not be solved for any value= 1..9, need to backtrack.
        puzzle[row][col] = 0;
        return kNotOK;
    }
    else
    {
        // No zeros found, all cells have a value and we are done.
        // This is the first E_OK status which will fulfill
        // if (kOK == ForwardSolver(puzzle)).
        return kOK;
    }
}

int PuzzleSolver::BackwardSolver(std::vector<std::vector<int>>& puzzle)
{
    int row{ 0 }, col{ 0 };

    // Increase counter for every recursion
    recursion_counter++;

    // Find first element with value 0
    if (kOK == GetFirstFreeElement(puzzle, row, col)) // <--- here iteration in row,col takes place
    {
        // Test all values from kMaxVal->1
        for (int value{ kMaxVal }; value >= 1; value--)
        {
            // Check if uiValue is valid for (row, col)
            if ((kOK == checker.IsInColValid(value, puzzle, row, col)) &&
                (kOK == checker.IsInRowValid(value, puzzle, row, col)) &&
                (kOK == checker.IsInGroupValid(value, puzzle, row, col)))
            {
                // Assign possible candidate for recursion N. */
                puzzle[row][col] = value;

                // Try to solve for recursion N+1 with the 'new' puzzle
                if (kOK == BackwardSolver(puzzle))
                {
                    // Puzzle solved (will generate an avalange of kOK).
                    // We will not end up here until it has been detected
                    // that no zero-elements can be found.
                    return kOK;
                }
            }
        }
        // Puzzle could not be solved for any value= 1..9, need to backtrack.
        puzzle[row][col] = 0;
        return kNotOK;
    }
    else
    {
        // No zeros found, all cells have a value and we are done.
        // This is the first E_OK status which will fulfill
        // if (kOK == BackwardSolver(puzzle))
        return kOK;
    }
}

int PuzzleSolver::MRVSolver(std::vector<std::vector<int>>& puzzle)
{
    int row{ 0 }, col{ 0 };

    // Increase counter for every recursion
    recursion_counter++;

    // Initialize stats data structure first call
    static bool called_once{ false };
    if (!called_once) 
    {
        InitializeStats(puzzle);
        called_once = true;
    }

    // Complete groups with 8 elements if present. Indexes needed if we need to
    // backtrack and value...
    //std::vector<std::tuple<int,int>> indexes = CompleteEigthElemGroups(puzzle);

    // Find the element with least possibilities
    if (kOK == GetElementMRV(puzzle, row, col))
    {
        // Test all values from 1->kMaxVal
        for (int value{ 1 }; value <= kMaxVal; value++)
        {
            // Check if uiValue is valid for (row, col)
            if ((kOK == checker.IsInColValid(value, puzzle, row, col)) &&
                (kOK == checker.IsInRowValid(value, puzzle, row, col)) &&
                (kOK == checker.IsInGroupValid(value, puzzle, row, col)))
            {
                // Assign possible candidate for recursion N. */
                puzzle[row][col] = value;

                // Add value to row_sum, col_sum and grp_sum.
                AddToRowSum(row, 1);
                AddToColSum(col, 1);
                AddToGrpSum(row, col, 1);

                // Try to solve for recursion N+1 with the 'new' puzzle
                if (kOK == MRVSolver(puzzle))
                {
                    // Puzzle solved (will generate an avalange of kOK).
                    // We will not end up here until it has been detected
                    // that no zero-elements can be found.
                    return kOK;
                }
                else
                {
                    // Remove value from row_sum, col_sum and grp_sum.
                    AddToRowSum(row, -1);
                    AddToColSum(col, -1);
                    AddToGrpSum(row, col, -1);
                }
            }
        }

        // Puzzle could not be solved for any value= 1..9, need to backtrack.

        // Revert assigned 8-element group completions
        //RevertEightElemGroupAssign(puzzle, indexes);

        puzzle[row][col] = 0;
        return kNotOK;
    }
    else
    {
        // No zeros found, all cells have a value and we are done.
        // This is the first E_OK status which will fulfill
        // if (kOK == MRVSolver(puzzle))
        return kOK;
    }
}

} // namespace puzzle_solver
