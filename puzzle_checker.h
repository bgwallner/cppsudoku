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
	int IsInRowValid(const int value, const std::vector<std::vector<int>>& puzzle,
		const int row, const int col);

	// Check if a value is valid in the current col in puzzle
	int IsInColValid(const int value, const std::vector<std::vector<int>>& puzzle,
		const int row, const int col);

	// Check that value is valid within 3x3 group
	int IsInGroupValid(const int value, const std::vector<std::vector<int>>& puzzle,
		const int row, const int col);

};

}  // namespace puzzle_checker

#endif


