#include "puzzle_checker.h"

#include "constants.h"

namespace puzzle_checker
{
PuzzleChecker::PuzzleChecker::PuzzleChecker() {}

PuzzleChecker::PuzzleChecker::~PuzzleChecker() {}

int PuzzleChecker:: IsInRowValid(const int value, const std::vector<std::vector<int>>& puzzle,
    const int row, const int col)
{
    // Boundary check
    if ((row >= kDim) || (col >= kDim))
    {
        return kNotOK;
    }

    for (int i{ 0 }; i < kDim; i++)
    {
        // Coloumn is running index
        if (i != col)
        {
            if (value == puzzle[row][i])
            {
                return kNotOK;
            }
        }
    }
    return kOK;
}

int PuzzleChecker::IsInColValid(const int value, const std::vector<std::vector<int>>& puzzle,
    const int row, const int col)
{
    // Boundary check
    if ((row >= kDim) || (col >= kDim))
    {
        return kNotOK;
    }

    for (int i{ 0 }; i < kDim; i++)
    {
        // Row is running index
        if (i != row)
        {
            if (value == puzzle[i][col])
            {
                return kNotOK;
            }
        }
    }
    return kOK;
}

int PuzzleChecker:: IsInGroupValid(const int value, const std::vector<std::vector<int>>& puzzle,
    const int row, const int col)
{
    int row_start{ 0 }, col_start{ 0 };

    // Boundary check
    if ((row >= kDim) || (col >= kDim))
    {
        return kNotOK;
    }

    // Find row and col start, e.g. group2 starts at col=3 and ends col=5
    // and 3 mod 3 = 0, 4 mod 3 = 1, 5 mod 3 = 2. Use this to find start.
    row_start = row - row % 3;
    col_start = col - col % 3;

    for (int i = row_start; i <= (row_start + 2); i++)
    {
        for (int j = col_start; j <= (col_start + 2); j++)
        {
            if (value == puzzle[i][j])
            {
                return kNotOK;
            }
        }
    }
    return kOK;
}
} // namespace puzzle_checker
