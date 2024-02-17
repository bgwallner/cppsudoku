#include "puzzle_solver.h"

namespace puzzle_solver
{

constexpr int kDim{ 9u };
constexpr int kMaxElems{ 81u };
constexpr int kMaxVal{ 9u };
constexpr int kOK{ 0 };
constexpr int kNotOK{ -1 };

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

PuzzleSolver::PuzzleSolver()
{
}

PuzzleSolver::~PuzzleSolver()
{
}

int PuzzleSolver::ForwardSolver(std::vector<std::vector<int>>& puzzle)
{
	return 0;
}

int PuzzleSolver::BackwardSolver(std::vector<std::vector<int>>& puzzle)
{
	return 0;
}

} // namespace puzzle_solver
