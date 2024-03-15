SUDOKU SOLVER & CREATOR
-----------------------

Description:

Sudoku solver using DFS by backtracking when no valid values can be found in the search space. 
The search space is limited by checking conditions for each recursion. The implementation is
separated into a number of classes for different purposes.

What can it do?

1) By provided text-input it reads sodoku from file and solves it.
2) Two plain DFS solvers are possible to use, the ForwardSolver evaluate values for each cell in order
   1->9 while BackwardSolver in order 9->1. This can be used to show that puzzles are unique.

   Reasoning: If we have found a solution to the provided puzzle where a cell [i,j] has the value
              5 we don't know if any value 6,7,8 or 9 would also generate a solution.

3) One MRV Solver, a DFS solver always choosing the next zero element having the least number
   of options including Heuristics to fill in cells having only one possible candidate. 
   This will decrease branching which can be seen by using the 17 clue input/gordonroyle.txt 
   where Forward- and Backward solver requires more than ~25 million recursions while MRV solver 
   use ~800 recursions.

4) A sudoku creator class where the number of clues are provided. It will randomly place random
   numbers 1..9 and and check constraint. If constraint is not met value will be wasted and we
   will continue until the requested number of clues has been placed. The puzzle will then be
   solved by ForwardSolver and then BackwardSolver. If any of them fail we start over from
   beginning.

Practical:

A Visual Studio project file is provided where optimization is turned on for release build. It drastically
increases performance. Solving time for one puzzle is in general ranging from a couple of milliseconds up to
250ms. If using GNU compile with g++ *.cpp and run created executable. Test-files can be found under input
directory.


    
