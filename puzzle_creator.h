#ifndef PUZZLE_CREATOR_H_
#define PUZZLE_CREATOR_H_

#include <vector>

namespace puzzle_creator {

class PuzzleCreator {
	// Sudoku will contain the puzzle to be solved
	std::vector<std::vector<int>> sudoku;

	// Solution will contain the solved sudoku
	std::vector<std::vector<int>> solution;

	// Runs the creation of the sodoku
	int RunPuzzleCreator(void);
public:
	PuzzleCreator() = delete;
	PuzzleCreator(const int NoOfClues);
	~PuzzleCreator();

	// Return a copy of the solved puzzle
	std::vector<std::vector<int>> GetPuzzleSolution(void);

	// Return sudoku puzzle
	std::vector<std::vector<int>> GetSudokuPuzzle(void);

};

}  // namespace puzzle_creator

#endif

