// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>

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
        //sudoku_fwd.ForwardSolver(v);
        sudoku_fwd.MRVSolver(v);
        
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
            std::cout << '\n';
        }
        else
        {
            std::cout << "RESULT: Puzzle is NOT unique.";
            std::cout << '\n';
        }
    }
    catch (const std::invalid_argument& e)
    {
        std::cout << e.what();
        std::cout << '\n';
    }

    std::cout << '\n';
    std::cout << " *** Creation of puzzle ***" << '\n';
    std::cout << '\n';

    int nbr_of_clues{ 0 };
    std::cout << "Enter number of clues in range 20->35: ";
    std::cin >> nbr_of_clues;
    std::cout << std::endl;
    try
    {
        // Measure time
        std::chrono::time_point<std::chrono::system_clock> start, end;
        start = std::chrono::system_clock::now();
        std::time_t start_time = std::chrono::system_clock::to_time_t(start);
    
        // Create a puzzle and its solution
        puzzle_creator::PuzzleCreator creator(nbr_of_clues);

        // End measurement
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);

        // Get the result
        std::vector<std::vector<int>> sudoku = creator.GetSudokuPuzzle();
        std::vector<std::vector<int>> solution = creator.GetPuzzleSolution();

        file_io::FileIO printer; // No file input
        printer.PrintPuzzleToConsole(sudoku);
        std::cout << '\n';
        printer.PrintPuzzleToConsole(solution);
        std::cout << '\n';
        std::cout << "Started computation at " << std::ctime(&start_time);
        std::cout << "Finished computation at " << std::ctime(&end_time)
                  << "Elapsed time: " << elapsed_seconds.count() << "s\n";
        std::cout << "Number of recursions needed:" << creator.GetNbrOfRecursions() << '\n';
        std::cout << "Number of not unique puzzles created:" 
                  << creator.GetNumberOfNotUnique() << '\n';
        std::cout << "Number of not solvable puzzles created:"
                  << creator.GetNumberOfNotSolved() << '\n';
        std::cout << "Ratio not unique/not solved:"
                  << 1.0*creator.GetNumberOfNotUnique()/creator.GetNumberOfNotSolved() << '\n';
        std::cout << "Average solving time per puzzle:"
                  << elapsed_seconds.count()*1000/(creator.GetNumberOfNotSolved()+creator.GetNumberOfNotUnique()) << " ms" << '\n';
    }
    catch (const std::invalid_argument& e)
    {
        std::cout << e.what();
    }
    return 0;
}

