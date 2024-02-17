#ifndef PUZZLE_SOLVER_H_
#define PUZZLE_SOLVER_H_

#include <vector>

namespace puzzle_solver {

class PuzzleSolver {
public:
	PuzzleSolver();
	~PuzzleSolver();

	// Solve puzzle using DFS by evaluating values 1->9
	int ForwardSolver(std::vector<std::vector<int>>& puzzle);

	// Solve puzzle using DFS by evaluating values 9->1
	int BackwardSolver(std::vector<std::vector<int>>& puzzle);

};

}  // namespace puzzle_solver

#endif
