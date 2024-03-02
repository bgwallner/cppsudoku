#ifndef PUZZLE_CREATOR_H_
#define PUZZLE_CREATOR_H_

#include <vector>

#include "puzzle_checker.h"
#include "puzzle_solver.h"

namespace puzzle_creator {

class PuzzleCreator {

	// Sudoku will contain the puzzle to be solved
	std::vector<std::vector<int>> sudoku;

	// Solution will contain the solved sudoku
	std::vector<std::vector<int>> solution;

	// Solution when solving with backward solver
	std::vector<std::vector<int>> solution_bw;

	// Number of clues provided in constructor
	int nbr_of_clues{0};

	// Metrics
	long nbr_not_solved{0};
	long nbr_not_unique{0};
	long nbr_of_recursions{0};

	// Runs the creation of the sodoku
	int RunPuzzleCreator(void);

	//Create a random sudoku-candidate
	int CreateRandomCandidate(puzzle_checker::PuzzleChecker& checker);

	// Set private members to 0
	int ClearPrivateData(void);

public:
	PuzzleCreator() = delete;
	PuzzleCreator(const int no_of_clues);
	~PuzzleCreator();

	// Return a copy of the solved puzzle
	std::vector<std::vector<int>> GetPuzzleSolution(void);

	// Return sudoku puzzle
	std::vector<std::vector<int>> GetSudokuPuzzle(void);

     // Metrics
	long GetNumberOfNotSolved() {return nbr_not_solved;};
	long GetNumberOfNotUnique() {return nbr_not_unique;};
	long GetNbrOfRecursions() {return nbr_of_recursions;};

};

}  // namespace puzzle_creator

#endif

