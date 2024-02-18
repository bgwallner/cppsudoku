#include "puzzle_creator.h"

#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "puzzle_solver.h"
#include "puzzle_checker.h"
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
    solution_bw.resize(kDim, std::vector<int>(kDim));

    NbrOfClues = NoOfClues;
	RunPuzzleCreator();
}

PuzzleCreator::~PuzzleCreator() {};

int PuzzleCreator::RunPuzzleCreator(void)
{
    bool unique{ false };
    puzzle_solver::PuzzleSolver solver;

    while (!unique)
    {
        // Start creating a random candidate
        CreateRandomCandidate();

        // Invoke solver
        if (kOK == solver.ForwardSolver(solution))
        {
            solver.BackwardSolver(solution_bw);
            if (solution == solution_bw)
            {
                // A unique Sudoku was 
                unique = true;
            }
            else
            {
                // Not unique, clear private data
                ClearPrivateData();
            }
        }
        else
        {
            // Not possible to solve, clear private data
            ClearPrivateData();
        }
    }
    return kOK;
}

int PuzzleCreator::ClearPrivateData(void)
{
    for (int i{ 0 }; i < kDim; i++)
    {
        for (int j{ 0 }; j < kDim; j++)
        {
            sudoku[i][j] = 0;
            solution[i][j] = 0;
            solution_bw[i][j] = 0;
        }
    }
    return kOK;
}

int PuzzleCreator::CreateRandomCandidate(void)
{
    int iterations{ 0 }, row{ 0 }, col{ 0 }, value{ 0 };
    puzzle_checker::PuzzleChecker checker;
    
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // use current time as seed for random generator
    while (iterations < NbrOfClues)
    {
        row = std::rand() % kDim;
        col = std::rand() % kDim;
        value = std::rand() % kDim + 1;

        /* Only place if value is zero */
        if (0 == sudoku[row][col])
        {
            /* Check if uiValue is valid for (uiRow, uiCol) */
            if ((kOK == checker.IsInColValid(value, sudoku, row, col)) &&
                (kOK == checker.IsInRowValid(value, sudoku, row, col)) &&
                (kOK == checker.IsInGroupValid(value, sudoku, row, col)))
            {
                sudoku[row][col] = value;
                solution[row][col] = value;
                solution_bw[row][col] = value;
                iterations++;
            }
        }
    }
    return kOK;
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
