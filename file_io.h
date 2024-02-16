#ifndef FILE_IO_H_
#define FILE_IO_H_

#include <vector>
#include <string>

namespace file_io {

class FileIO {

	// Puzzle declaration
	std::vector<std::vector<int>> puzzle;

	// Class private method invoked from overloaded constructor
	// which reads data into private member puzzle
	int ReadPuzzleFromFile(std::string file_name);

	// Validate that puzzle is 9x9
	int ValidatePuzzle(const std::vector<std::vector<int>>& puzzle);
public:

	// Default constructor
	FileIO();

	// Constructor reading from file_name and fills class
	// private member puzzle 
	FileIO(std::string file_name);
	~FileIO();

	// Return a copy of the puzzle
	std::vector<std::vector<int>> GetPuzzle(void);

	// Print the provided puzzle to console
	int PrintPuzzleToConsole(const std::vector<std::vector<int>>& puzzle);

	// Print class private puzzle
	int PrintPuzzleToConsole(void);
};

}  // namespace file_io

#endif
