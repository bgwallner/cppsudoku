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

        // Print the fetched puzzle
        puzzle.PrintPuzzleToConsole(v);
        std::cout << std::endl;

        // Print current puzzle stored in class
        puzzle.PrintPuzzleToConsole();
        std::cout << std::endl;
    }
    catch (const std::invalid_argument& e)
    {
        std::cout << e.what();
        return 0;
    }

    // Invoke default constructor (no file read)
    file_io::FileIO puzzle2;
    puzzle2.PrintPuzzleToConsole();
    
    return 0;
}

