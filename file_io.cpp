#include "file_io.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>

namespace file_io {

constexpr int kDim{ 9u };
constexpr int kMaxElems{ 81u };
constexpr int kMaxVal{ 9u };
constexpr int kOK{ 0 };
constexpr int kNotOK{ -1 };
	
int FileIO::ReadPuzzleFromFile(std::string file_name)
{
    char a{};
    int value{ 0 };
    int elem_counter{ 0 };

    // Accepted input is numbers {1,9} and '.'
    std::fstream infile(file_name);
    while ((infile >> a) && (kMaxElems > elem_counter))
    {
        value = a - '0';
        if ((value >= 1) && (value <= kMaxVal))
        {
            puzzle[elem_counter/kDim][elem_counter%kDim] = value;
        }
        else if (a != '.')
        {
            return kNotOK;
        }
        else
        {
            // Do nothing
        }
        elem_counter++;
    }
	return kOK;
}

FileIO::FileIO()
{
    // Resize into 9x9 matrix
    puzzle.resize(kDim, std::vector<int>(kDim));
}

FileIO::FileIO(std::string file_name)
{
	// Resize into a 9x9 matrix.
	puzzle.resize(kDim, std::vector<int>(kDim));

	// Read the puzzle from file_name
    if (ReadPuzzleFromFile(file_name) != kOK)
    {
        throw std::invalid_argument("Invalid file syntax.");
    }
}

FileIO::~FileIO() 
{
    // Nothing to do
}

std::vector<std::vector<int>> FileIO::GetPuzzle(void)
{
	return puzzle;
}

int FileIO::ValidatePuzzleDim(const std::vector<std::vector<int>>& puzzle)
{
    if (kDim == puzzle.size())
    {
        for (int i{ 0 }; i < kDim; i++)
        {
            if (kDim != puzzle[i].size())
            {
                return kNotOK;
            }
        }
    }
    else
    {
        return kNotOK;
    }
    return kOK;
}
int FileIO::PrintPuzzleToConsole(const std::vector<std::vector<int>>& puzzle)
{
    // Check puzzle dimensions for provided puzzle
    if (kOK != FileIO::ValidatePuzzleDim(puzzle))
    {
        return kNotOK;
    }

    std::cout << "                  SODUKU PUZZLE\n";
    std::cout << "\n";
    std::cout <<"  --------------------------------------------\n";
    for (int row{ 0 }; row < kDim; row++)
    {
        for (int col{ 0 }; col < kDim; col++)
        {
            if ((kDim - 1) == col)
            {
                std::cout << "  | " << puzzle[row][col] << " | ";
            }
            else
            {
                std::cout << "  | " << puzzle[row][col];
            }
        }
        std::cout << "\n";
        std::cout << "  --------------------------------------------\n";
    }
    return kOK;
}

int FileIO::PrintPuzzleToConsole(void)
{
    std::cout << "                  SODUKU PUZZLE\n";
    std::cout << "\n";
    std::cout << "  --------------------------------------------\n";
    for (int row{ 0 }; row < kDim; row++)
    {
        for (int col{ 0 }; col < kDim; col++)
        {
            if ((kDim - 1) == col)
            {
                std::cout << "  | " << puzzle[row][col] << " | ";
            }
            else
            {
                std::cout << "  | " << puzzle[row][col];
            }
        }
        std::cout << "\n";
        std::cout << "  --------------------------------------------\n";
    }
    return kOK;
}

} // namespace file_io