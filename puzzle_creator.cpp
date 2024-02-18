#include "puzzle_creator.h"

#include <stdexcept>
#include <iostream>

#include "puzzle_solver.h"
#include "constants.h"

namespace puzzle_creator
{

constexpr int kMaxClues{ 35u };
constexpr int kMinClues{ 20u };

PuzzleCreator::PuzzleCreator(const int NoOfClues)
{
	if (kMaxClues < NoOfClues || kMinClues > NoOfClues)
	{
		throw std::invalid_argument("ERROR: Number of clues out of range {20..35}.");
		std::cout << "\n";
	}
	// Resize into 9x9 matrix
	solution.resize(kDim, std::vector<int>(kDim));
	sudoku.resize(kDim, std::vector<int>(kDim));
	RunPuzzleCreator();
}

int PuzzleCreator::RunPuzzleCreator(void)
{
	return 0;
}

PuzzleCreator::~PuzzleCreator()
{
}

std::vector<std::vector<int>> PuzzleCreator::GetPuzzleSolution(void)
{
	return solution;
}

std::vector<std::vector<int>> PuzzleCreator::GetSudokuPuzzle(void)
{
	return sudoku;
}

} //namespace puzzle_creator
