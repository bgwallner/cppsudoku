#ifndef PUZZLE_SOLVER_H_
#define PUZZLE_SOLVER_H_

#include <vector>

namespace puzzle_solver {

class PuzzleSolver {

long recursion_counter{ 0 };

// Get the first available element (value=0)
int GetFirstFreeElement(const std::vector<std::vector<int>>& puzzle,
	int& row, int& col);

public:
	PuzzleSolver();
	~PuzzleSolver();

	// Solve puzzle using DFS by evaluating values 1->9
	int ForwardSolver(std::vector<std::vector<int>>& puzzle);

	// Solve puzzle using DFS by evaluating values 9->1
	int BackwardSolver(std::vector<std::vector<int>>& puzzle);

	// Get number of recursions needed for solver
	long GetNumberOfRecursions(void) { return recursion_counter; }

};

}  // namespace puzzle_solver

#endif
