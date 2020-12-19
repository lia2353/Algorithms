## Homework1: Sliding Blocks(N Puzzle)

•The gamestarts with a board consisting of blocks numbered 1 through N and one blank block represented by the number 0. 
The goal is to arrange the tiles according to their numbers. Moving is done by moving the blocks on top, bottom, left and right in place of the empty block.<br>
•At the input is given the number N -the number of blocks with numbers (8, 15, 24, etc.), the number I - the index of the position of zero (the empty block) 
in the decision (using -1 the default zero index position is set at the bottom right) and then the layout of the board is introduced.
<br>

Using the A*(or IDA*) algorithm and the Manhattan distance heuristics (or Hemming distance), derive: <br>
•In the first linae, the length of the "optimal" path from start to destination.<br>
•The appropriate steps (in a new line for each one) that are taken to reach the final state. The steps are left, right, upand down <br>
•Keep in mind that not every puzzle is solvable. You can check whether the puzzle is solvable here ( https://www.cs.princeton.edu/courses/archive/spring18/cos226/assignments/8puzzle/index.html ) or directly use valid examples.

Sample input:<br>
8<br>
-1<br>
1 2 3<br>
4 5 6<br>
0 7 8<br>

Sample output:<br>
2<br>
left<br>
left<br>

<br>


## Homework2: N Queens

•Place N chess queens on an N×N chessboard so that no two queens threaten each other. In other words the solution requires that no two queens share the same row, column, or diagonal.<br>
•Use the MinConflicts algorithm to solve the problem.<br>

•Input: Integer N - the number of queens to be located.<br>
`*` Requirement to work for N = 10,000<br>
•Output: Print in the console the board by desigating a queen with * and an empty cell with _.<br>

Sample input:<br>
4<br>

Sample output:<br>
 `_ * _ _` <br>
 `_ _ _ *` <br>
 `* _ _ _` <br>
 `_ _ * _` <br>
 
N Queens: Pseudocode<br>
```
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
```

## Homework3: Traveling Salesman Problem
Да се реши задачата за търговския пътник (Traveling Salesman Problem) чрез използване на генетичен алгоритъм.<br>

За целта на програмата се задава: N - число (N <= 100) - брой точки в пространството (брой градове).<br>

Програмата генерира N точки от двумерна координатна система на случаен принцип.<br>

Търси се най-къс път, който да минава през всяка точка само по един път. За целта нека да се изведе на пет стъпки дължината на текущо най-добрия път в популацията.<br>
    1. на 10-та генерация<br>
    2, 3, 4 - по избор<br>
    5. След последната генерация.<br>

## Homework4: Tic-Tac-Toe
Да се имплементира игра на морски шах срещу противник, като се използва алгоритъм **min-max** с **alpha-beta отсичане**.

Започва се с празна дъска.
На всяка стъпка:
- единият играч въвежда две числа ([1,3]) от клавиатура, които са неговия ход на дъската;  
- използвайки алгоритъма,  намираме и нашия ход;
- след това се показва конкретното състояние на играта.  
Накрая се показва кой е спечелил играта.  
  
Изисквания:
- Направете играта така, че да може да се задава дали компютърът или играчът е първи.  
- Направете алгоритъма оптимален. 
