#include "puzzle_solver.h"

#include <tuple>
#include <algorithm>
#include <numeric>

#include "constants.h"
#include "file_io.h"

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

// Return the value for row, col if there is only one that can be assigned
int ReturnIfInUnitOnlyOneLeft(const std::vector<std::vector<int>>& puzzle,
    const int row, const int col)
{
    std::vector<int> value_found{};

    int row_start{ 0 }, col_start{ 0 };

    row_start = row - row % 3;
    col_start = col - col % 3;

    // Add all elements in grp not being zero to value_found
    for (int row_{ row_start }; row_ <= (row_start + 2); row_++)
    {
        for (int col_{ col_start }; col_ <= (col_start + 2); col_++)
        {
            if (puzzle[row_][col_] != 0)
            {
                value_found.push_back(puzzle[row_][col_]);
            }
        }
    }

    // Add values found in row
    for (int row_{ 0 }; row_ < kDim; row_++)
    {
        // Row is running index, don't count values within grp
        if (!(row_ >= row_start && row_ <= (row_start + 2)))
        {
            if (puzzle[row_][col] != 0)
            {
                value_found.push_back(puzzle[row_][col]);
            }
        }
    }

    // Add values found in col
    for (int col_{ 0 }; col_ < kDim; col_++)
    {
        // Row is running index
        if (!(col_ >= col_start && col_ <= (col_start + 2)))
        {
            if (puzzle[row][col_] != 0)
            {
                value_found.push_back(puzzle[row][col_]);
            }
        }
    }

    // If values found is less than 8 there can't be just one value left
    std::vector<int> vals_histogram{ 0,0,0,0,0,0,0,0,0 };
    int value = 0;
    if (value_found.size() >= 8)
    {
        for (int i{ 0 }; i < value_found.size(); i++)
        {
            // Value to index mapping
            vals_histogram[value_found[i]-1]++;
        }

        // Check number of zero elements, if more than 1 we are f#cked.
        int counter{ 0 };
        for (int i{ 0 }; i < vals_histogram.size(); i++)
        {
            if (vals_histogram[i] == 0)
            {
                counter++;
            }
        }

        // Check if only one zero
        if (counter == 1)
        {
            // Iterator to element
            auto it = std::find(vals_histogram.begin(), vals_histogram.end(), 0);
            value = static_cast<int>(std::distance(vals_histogram.begin(), it) + 1);
        }
    }
    return value;
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

void PuzzleSolver::RevertSoleCandidate(std::vector<std::vector<int>>& puzzle,
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

std::vector<std::tuple<int,int>> PuzzleSolver::AddSoleCandidate(std::vector<std::vector<int>>& puzzle)
{
    std::vector<std::tuple<int,int>> sole_candidates {};
    for (int row{ 0 }; row < kDim; row++)
    {
        for (int col{ 0 }; col < kDim; col++)
        {
            if (puzzle[row][col] == 0)
            {
                // Check if there is only one being possible to assign
                int value = ReturnIfInUnitOnlyOneLeft(puzzle, row, col);

                // Check if only one value was missing
                if (value != 0)
                {
                    //file_io::FileIO printer;
                    //printer.PrintPuzzleToConsole(puzzle);
                    // Assign value
                    puzzle[row][col] = value;
                    // Update stats
                    AddToRowSum(row, 1);
                    AddToColSum(col, 1);
                    AddToGrpSum(row, col, 1);

                    // Save row, col if we need to revert
                    std::tuple<int, int> temp{ row,col };
                    sole_candidates.push_back(temp);
                }
            }
        }
    }
    return sole_candidates;
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

    // Heuristics - Human reasoning
    
    // 1) Sole Candidate: Complete cells having only one value possible to set.
    std::vector<std::tuple<int,int>> indexes = AddSoleCandidate(puzzle);

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
        RevertSoleCandidate(puzzle, indexes);

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
