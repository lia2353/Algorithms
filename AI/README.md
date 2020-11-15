Homework: “Sliding Blocks(N Puzzle)”
•The gamestarts with a board consisting of blocks numbered 1 through N and one blank block represented by the number 0. 
The goal is to arrange the tiles according to their numbers. Moving is done by moving the blocks on top, bottom, left and right in place of the empty block.
•At the input is given the number N -the number of blocks with numbers (8, 15, 24, etc.), the number I - the index of the position of zero (the empty block) 
in the decision (using -1 the default zero index position is set at the bottom right) and then the layout of the board is introduced. 

Using the A*(or IDA*) algorithm and the Manhattan distance heuristics (or Hemming distance), derive:
•In the first line, the length of the "optimal" path from start to destination.
•The appropriate steps (in a new line for each one) that are taken to reach the final state. The steps are left, right, upand down
•Keep in mind that not every puzzle is solvable. You can check whether the puzzle is solvable here (https://www.cs.princeton.edu/courses/archive/spring18/cos226/assignments/8puzzle/index.html)
or directly use valid examples.

Sample input:
8
-1
1 2 3
4 5 6
0 7 8

Sample output:
2
left
left




Homework: N Queens
•Place N chess queens on an N×N chessboard so that no two queens threaten each other. In other words the solution requires that no two queens share the same row, column, or diagonal.
•Use the MinConflicts algorithm to solve the problem.

•Input: Integer N - the number of queens to be located.
* Requirement to work for N = 10,000
•Output: Print in the console the board by desigating a queen with * and an empty cell with _.

Sample input:
4

Sample output:
_ * _ _ 
_ _ _ *
* _ _ _
_ _ * _

N Queens: Pseudocode
solve(N) {
  // Putting the queen on the row with min conflicts
  queens[] = init(N)
  iter= 0
  while(iter++ <= k*N) {
    // Randomly if two or more!
    col = getColWithQueenWithMaxConf()
    // Randomly if two or more!
    row = getRowWithMinConflict(col)
    // index=col & value=row
    queens[col] = row
  }
  if(hasConflicts()) {
    // Restart
    solve(N)
  }
}
