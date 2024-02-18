#ifndef PUZZLE_CREATOR_H_
#define PUZZLE_CREATOR_H_

#include <vector>

namespace puzzle_creator {

class PuzzleCreator {
	// Sudoku will contain the puzzle to be solved
	std::vector<std::vector<int>> sudoku;

	// Solution will contain the solved sudoku
	std::vector<std::vector<int>> solution;

	// Solution when solving with backward solver
	std::vector<std::vector<int>> solution_bw;

	// Number of clues provided in constructor
	int NbrOfClues{ 0 };

	// Runs the creation of the sodoku
	int RunPuzzleCreator(void);

	//Create a random sudoku-candidate
	int CreateRandomCandidate(void);

	// Set private members to 0
	int ClearPrivateData(void);

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

