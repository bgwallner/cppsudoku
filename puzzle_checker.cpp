#include "puzzle_checker.h"

namespace puzzle_checker
{

constexpr int kDim{ 9u };
constexpr int kMaxElems{ 81u };
constexpr int kMaxVal{ 9u };
constexpr int kOK{ 0 };
constexpr int kNotOK{ -1 };

PuzzleChecker::PuzzleChecker::PuzzleChecker() {}

PuzzleChecker::PuzzleChecker::~PuzzleChecker() {}

int PuzzleChecker:: IsInRowValid(int value, const std::vector<std::vector<int>>& puzzle,
    int row, int col)
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

int PuzzleChecker::IsInColValid(int value, const std::vector<std::vector<int>>& puzzle,
    int row, int col)
{
    // Boundary check
    if ((row >= kDim) || (col >= kDim))
    {
        return kNotOK;
    }

    for (int i{ 0 }; i < kDim; i++)
    {
        // Row is running index
        if (i != col)
        {
            if (value == puzzle[i][col])
            {
                return kNotOK;
            }
        }
    }
    return kOK;
}

int PuzzleChecker:: IsInGroupValid(int value, const std::vector<std::vector<int>>& puzzle,
    int row, int col)
{
    int row_start{ 0 }, col_start{ 0 };

    // Boundary check
    if ((row >= kDim) || (col >= kDim))
    {
        return kNotOK;
    }

    /* Find row and col start, e.g. group2 starts at col=3 and ends col=5 */
    /* and 3 mod 3 = 0, 4 mod 3 = 1, 5 mod 3 = 2. Use this to find start. */
    row_start = row - row % 3;
    col_start = col - col % 3;

    for (int i = row_start; i < (row_start + 3); i++)
    {
        for (int j = col_start; j < (col_start + 3); j++)
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
