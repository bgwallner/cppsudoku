#ifndef PUZZLE_CHECKER_H_
#define PUZZLE_CHECKER_H_

#include <vector>

namespace puzzle_checker {

class PuzzleChecker {
public:
	// Default constructor
	PuzzleChecker();

	// Destructor
	~PuzzleChecker();

	// Check if a value is valid in the current row in puzzle
	int IsInRowValid(int value, const std::vector<std::vector<int>>& puzzle, int row, int col);

	// Check if a value is valid in the current col in puzzle
	int IsInColValid(int value, const std::vector<std::vector<int>>& puzzle, int row, int col);

	int IsInGroupValid(int value, const std::vector<std::vector<int>>& puzzle, int row, int col);

};

}  // namespace puzzle_checker

#endif


