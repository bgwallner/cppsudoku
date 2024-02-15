#include "file_io.h"
#include <iostream>

namespace file_io {

constexpr int kDim{ 9u };
	
int FileIO::ReadPuzzleFromFile(std::string file_name)
{
	return 0;
}

FileIO::FileIO()
{
}

FileIO::FileIO(std::string file_name)
{
	// Resize into a 9x9 matrix.
	puzzle.resize(kDim, std::vector<int>(kDim));

	// Read the puzzle from file_name
	ReadPuzzleFromFile(file_name);
}

FileIO::~FileIO() {
	// Resize into 9x9 matrix
	puzzle.resize(kDim, std::vector<int>(kDim));

}

std::vector<std::vector<int>> FileIO::GetPuzzle(void)
{
	// TODO: insert return statement here
	return puzzle;
}

int FileIO::PrintPuzzleToConsole(const std::vector<std::vector<int>>& puzzle)
{
    std::cout << "                  SODUKU PUZZLE\n";
    std::cout << "\n";
    std::cout <<"  --------------------------------------------\n";
    for (int row = 0; row < kDim; row++)
    {
        for (int col = 0; col < kDim; col++)
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
	return 0;
}

int PrintPuzzleToConsole(void)
{
    std::cout << "                  SODUKU PUZZLE\n";
    std::cout << "\n";
    std::cout << "  --------------------------------------------\n";
    for (int row = 0; row < kDim; row++)
    {
        for (int col = 0; col < kDim; col++)
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
    return 0;
}

} // namespace file_io