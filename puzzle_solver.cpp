#include "puzzle_solver.h"

#include "constants.h"

namespace puzzle_solver
{
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

int PuzzleSolver::GetGroupSum(const int row, const int col)
{
    // TODO: Based on col, row calculate box which grp 1-9
    return 0;
}

void PuzzleSolver::AddToGrpSum(const int row, const int col, const int value)
{
    // TODO: Based on col, row add value to the relevant grp 1-9
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

    // Find first element being most constrained
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
                AddToRowSum(row, value);
                AddToColSum(col, value);
                AddToGrpSum(row, col, value);

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
                    AddToRowSum(row, -value);
                    AddToColSum(col, -value);
                    AddToGrpSum(row, col, -value);
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
        // if (kOK == MRVSolver(puzzle))
        return kOK;
    }
}

} // namespace puzzle_solver
