// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>

#include "file_io.h"
#include "puzzle_checker.h"
#include "puzzle_solver.h"
#include "puzzle_creator.h"

int main()
{
    std::string file_name;
    std::cout << "Enter filename: ";
    std::cin >> file_name;
    std::cout<<std::endl;

    // Create a puzzle from input
    try {
        file_io::FileIO puzzle{ file_name };

        // Get current stored puzzle in class
        std::vector<std::vector<int>> v = puzzle.GetPuzzle();

        // Make a copy of the puzzle
        std::vector<std::vector<int>> u = v;

        // Print the fetched puzzle
        puzzle.PrintPuzzleToConsole(v);
        std::cout << '\n';

        // Solve the puzzle forward solver
        puzzle_solver::PuzzleSolver sudoku_fwd;
        sudoku_fwd.ForwardSolver(v);
        
        // Print the solved puzzle
        std::cout << "*** Solving puzzle with forward solver ***" << '\n';
        std::cout << '\n';
        puzzle.PrintPuzzleToConsole(v);
        std::cout << '\n';
        std::cout << "Number of recursions needed : " << sudoku_fwd.GetNumberOfRecursions() << '\n';
        std::cout << '\n';

        // Solve the puzzle backward solver
        puzzle_solver::PuzzleSolver sudoku_bw;
        sudoku_bw.BackwardSolver(u);

        // Print the solved puzzle
        std::cout << "*** Solving puzzle with backward solver ***" << '\n';
        std::cout << '\n';
        puzzle.PrintPuzzleToConsole(u);
        std::cout << '\n';
        std::cout << "Number of recursions needed : " << sudoku_bw.GetNumberOfRecursions() << '\n';
        std::cout << '\n';

        // Check if solutions are the same (=unique)
        if (v == u)
        {
            std::cout << "RESULT: Puzzle is unique";
        }
        else
        {
            std::cout << "RESULT: Puzzle is NOT unique.";
        }
    }
    catch (const std::invalid_argument& e)
    {
        std::cout << e.what();
        return 0;
    }

    return 0;
}

