#ifndef PUZZLE_SOLVER_H_
#define PUZZLE_SOLVER_H_

#include <vector>

#include "puzzle_checker.h"

namespace puzzle_solver {

// The struct stats keep track of sums for row, col and group.
// Initially it will be filled by going through the empty puzzle
// containing clues only. During recursion it will be updated
// continously when adding a new value to the puzzle. When puzzle
// is solved all elements will be 9.

// Example) If row[3] = 1 0 0 5 4 2 0 0 0 we will have 
//          row_sums = [0 0 0 4 0 0 0 0 0]
struct HeuristicStats {
	std::vector<int> row_sums{ 0,0,0,0,0,0,0,0,0 };
	std::vector<int> col_sums{ 0,0,0,0,0,0,0,0,0 };
	std::vector<int> grp_sums{ 0,0,0,0,0,0,0,0,0 };
};

class PuzzleSolver {

	// Used for measuring number of recursions
	long recursion_counter{ 0 };

	// Private member for keeping track of
	// row, col and grp sums
	HeuristicStats stats;

	// Get the first available element (value=0)
	int GetFirstFreeElement(const std::vector<std::vector<int>>& puzzle,
		int& row, int& col);

	// Initialize the stats data structure. Used during first invocation
	// in MRVSolver.
	void InitializeStats(const std::vector<std::vector<int>>& puzzle);

	// Calculates sum of row (used on empty puzzle with clues)
	int GetRowSum(const int row) { return stats.row_sums[row]; };

	// Adds value to row
	void AddToRowSum(const int row, const int value) { stats.row_sums[row] += value; };

	// Calculates sum of col  (used on empty puzzle with clues)
	int GetColSum(const int col) { return stats.col_sums[col]; };

	// Adds value to col
	void AddToColSum(const int col, const int value) { stats.col_sums[col] += value; };

	// Calculates sum for group with row, col  (used on empty puzzle with clues)
	int GetGroupSum(const int row, const int col);

	// Adds value to grp
	void AddToGrpSum(const int row, const int col, const int value);

	// Gets element with Minimum Remaining Value
	int GetElementMRV(const std::vector<std::vector<int>>& puzzle,
		int& row, int& col);

	// Complete the cells having only one element left to set
	std::vector<std::tuple<int,int>> CandidateReduction(std::vector<std::vector<int>>& puzzle);

	// Revert assigned from CompleteEigthElemGroups
    void RevertCandidateReduction(std::vector<std::vector<int>>& puzzle,
	const std::vector<std::tuple<int,int>>& indexes);

    // Create checker object (otherwise created
	// recursively in DFS)
	puzzle_checker::PuzzleChecker checker;

public:
	PuzzleSolver();
	~PuzzleSolver();

	// Solve puzzle using DFS by evaluating values 1->9
	int ForwardSolver(std::vector<std::vector<int>>& puzzle);

	// Solve puzzle using DFS by evaluating values 9->1
	int BackwardSolver(std::vector<std::vector<int>>& puzzle);

	// Solve puzzle using Minimum Remaining Value strategy
	int MRVSolver(std::vector<std::vector<int>>& puzzle);

	// Get number of recursions needed for solver
	long GetNumberOfRecursions(void) { return recursion_counter; }

	// Reset recursion counter
	void ResetRecursionCounter(void) { recursion_counter=0;};

};

}  // namespace puzzle_solver

#endif
