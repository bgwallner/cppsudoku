#include "puzzle_creator.h"

#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>

#include "constants.h"

namespace puzzle_creator
{

constexpr int kMaxClues{ 35u };
constexpr int kMinClues{ 20u };

PuzzleCreator::PuzzleCreator(const int no_of_clues)
{
	if (kMaxClues < no_of_clues || kMinClues > no_of_clues)
	{
		throw std::invalid_argument("ERROR: Number of clues out of range {20..35}.");
		std::cout << "\n";
	}
	// Resize into 9x9 matrix
	solution.resize(kDim, std::vector<int>(kDim));
	sudoku.resize(kDim, std::vector<int>(kDim));
    solution_bw.resize(kDim, std::vector<int>(kDim));

    nbr_of_clues = no_of_clues;
	RunPuzzleCreator();
}

PuzzleCreator::~PuzzleCreator() {};

int PuzzleCreator::RunPuzzleCreator(void)
{
    bool unique{ false };
    puzzle_checker::PuzzleChecker checker;
    puzzle_solver::PuzzleSolver solver;

    while (!unique)
    {
        // Start creating a random candidate
        CreateRandomCandidate(checker);

        // Invoke solver
        if (kOK == solver.ForwardSolver(solution))
        {
            // Only use counter from forward solving
            nbr_of_recursions = solver.GetNumberOfRecursions();

            // Solve backwards to check uniqueness
            solver.BackwardSolver(solution_bw);
            if (solution == solution_bw)
            {
                // A unique Sudoku was found
                unique = true;
            }
            else
            {
                // Not unique, clear private data
                nbr_not_unique++;
                ClearPrivateData();
                solver.ResetRecursionCounter();
            }
        }
        else
        {
            // Not possible to solve, clear private data
            nbr_not_solved++;
            ClearPrivateData();
            solver.ResetRecursionCounter();
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

int PuzzleCreator::CreateRandomCandidate(puzzle_checker::PuzzleChecker& checker)
{
    int iterations{ 0 }, row{ 0 }, col{ 0 }, value{ 0 };
    //puzzle_checker::PuzzleChecker checker; // <--- remove
    
    std::uniform_int_distribution<int> distribution(0, 8);
    std::random_device rd;
    std::mt19937 engine(rd());
    //std::srand(static_cast<unsigned int>(std::time(nullptr))); // use current time as seed for random generator
    while (iterations < nbr_of_clues)
    {
        row = distribution(engine);
        col = distribution(engine);
        value = distribution(engine) + 1;

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
