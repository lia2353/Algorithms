## Homework 1: Sliding Blocks (N Puzzle)

[The game](http://mypuzzle.org/sliding) starts with a board consisting of blocks numbered 1 through N and one blank block represented by the number 0. 
The goal is to arrange the tiles according to their numbers. Moving is done by moving the blocks on top, bottom, left and right in place of the empty block.

At the input is given: the number N - the count of blocks with numbers (8, 15, 24, etc.), the number I - the index of the position of zero (the empty block) in the decision (using -1 the default zero index position is set at the bottom right) and then the layout of the board is introduced.

Using the **IDA*** algorithm and the **Manhattan distance heuristics** (or Hemming distance), derive:
- In the first line, the length of the "optimal" path from start to destination.
- The appropriate steps (new line for each one) that are taken to reach the final state. The steps are **left, right, upand down**. <br>
`*` Keep in mind that not every puzzle is solvable. You can check whether the puzzle is solvable [here](https://www.cs.princeton.edu/courses/archive/spring18/cos226/assignments/8puzzle/index.html) or directly use valid examples.

Sample input:
```
8
-1
1 2 3
4 5 6
0 7 8
````

Sample output:
```
2
left
left
````


## Homework 2: N Queens

Place **N** chess queens on an N×N chessboard so that no two queens threaten each other. In other words the solution requires that no two queens share the same row, column, or diagonal.
Use the **MinConflicts algorithm** to solve the problem.

- **Input:** Integer N - the number of queens to be located.<br>
`*` Requirement to work for N = 10,000.
- **Output:** Print in the console the board by desigating a queen with * and an empty cell with _.

Sample input:
````
4
````
Sample output:
````
 _ * _ _
 _ _ _ *
 * _ _ _
 _ _ * _
````
 
N Queens: Pseudocode
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

**Additional:** reverseNQueens file -> returns board with queens aranged in max conflicts configuration

## Homework 3: Traveling Salesman Problem (TSP)
Да се реши задачата за **търговския пътник** (Traveling Salesman Problem) чрез използване на **генетичен алгоритъм**.

За целта на програмата се задава: N - число (N <= 100) - брой точки в пространството (брой градове).
Програмата генерира N точки от двумерна координатна система на случаен принцип.

Търси се най-къс път, който да минава през всяка точка само по един път. За целта нека да се изведе на пет стъпки дължината на текущо най-добрия път в популацията.<br>
    1. на 10-та генерация<br>
    2, 3, 4 - по избор<br>
    5. След последната генерация.<br>
    
Genetic Algorithm:
```
1. Initialize the population randomly.
2. Determine the fitness of the chromosome.
3. Until done repeat:
    1. Select parents.
    2. Perform crossover and mutation.
    3. Calculate the fitness of the new population.
    4. Append it to the gene pool.
```
In my implemetantion, I have 1000 **individuals** in each generation, keep 100 **elite individuals**, crossover 500 **parents** to create 900 **children**, use a 0.1 **mutation rate** for a given gene, and run through 1000 **generations**.

## Homework 4: Tic-Tac-Toe
Да се имплементира игра на **морски шах** срещу противник, като се използва алгоритъм **min-max** с **alpha-beta отсичане** (Minimax algorithm with alpa-beta pruning).

Започва се с празна дъска.
На всяка стъпка:
- единият играч въвежда две числа в интервала ([1,3]) от клавиатура, които са неговия ход на дъската;  
- използвайки алгоритъма,  намираме и нашия ход;
- след това се показва конкретното състояние на играта.  
Накрая се показва кой е спечелил играта.  
  
Requirements:
- Must be able to choose who is the first player.
- The algorithm must be optimal. (The fastest way to win => min depth)

Minimax: Pseudocode
```
function minimax(node, depth, isMaximizingPlayer, alpha, beta):

    if node is a leaf node :
        return value of the node
    
    if isMaximizingPlayer :
        bestVal = -INFINITY 
        for each child node :
            value = minimax(node, depth+1, false, alpha, beta)
            bestVal = max( bestVal, value) 
            alpha = max( alpha, bestVal)
            if beta <= alpha:
                break
        return bestVal

    else :
        bestVal = +INFINITY 
        for each child node :
            value = minimax(node, depth+1, true, alpha, beta)
            bestVal = min( bestVal, value) 
            beta = min( beta, bestVal)
            if beta <= alpha:
                break
        return bestVal
 ```
 The algorithm is optimal. We can test it with this board:
The current board is this and is Computer's turn:
```
O| |X|
X| | |
X|O|O|
```
If the algorithm doesn't consider the depth (i.e. is not optimal), next move will be
```
O|X|X|
X| | |
X|O|O|
```
If the algorithm is optimal (i.e. depth included), next move will be:
```
O| |X|
X|X| |
X|O|O|
```

**Additional:** reverse logic (commented code) -> The Computer plays worst move. In this case Computer and Human sre always DRAW.

## Homework 5: Naive Bayes Classifier
Реализирайте **Наивен Бейсов Класификатор**, класифициращ индивидите като демократи или републиканци, използвайки 16-те атрибута и двата класа от [следните данни.](http://archive.ics.uci.edu/ml/datasets/Congressional+Voting+Records) (* Данните може да имат нужда от предварителна обработка.)

За тестване на алгоритъма приложете 10-fold крос-валидация (разделете данните по случаен начин на 10 множества и направете 10 обучения и тествания на модела като всеки път използвате едно от множествата за тестване, а останалите 9 за обучение).

Изведете метриката точност (Accuracy) за всяко от 10-те обучения, както и тяхното средно аритметично (за обобщена оценка на класификатора).

- Laplace Smoothing - За да решим проблема с нулевите вероятности. Това, че нещо е с вероятност 0 според дадените данни, не значи, че  еневъзможно, а че е малко възможно. Затова го преработваме така, че да е малко вероятно.
- Log probabilities - За да решим проблема с умножаването на вероятности, което води до числа като 0,00..002 например. Затова използваме свойството на логаритмите ln(a·b) = lna + lnb и получаваме удобни за сметки числа и запазват знака за сравнение. (Внимание: Понеже вероятностите са между 0 и 1, то логаритмите от тези вероятности ще бъдат винаги отрицателни числа)

Пoлезни връзки:
- [ShatterLine Blog](http://shatterline.com/blog/2013/09/12/not-so-naive-classification-with-the-naive-bayes-classifier/)
- [GeeksforGeeks](https://www.geeksforgeeks.org/naive-bayes-classifiers/)
- [K-fold Cross-Validation](https://towardsdatascience.com/the-importance-of-k-fold-cross-validation-for-model-prediction-in-machine-learning-4709d3fed2ef)
- [Laplace Smoothing](https://towardsdatascience.com/introduction-to-na%C3%AFve-bayes-classifier-fa59e3e24aaf)
- [Log-probabilities](http://www.cs.rhodes.edu/~kirlinp/courses/ai/f18/projects/proj3/naive-bayes-log-probs.pdf)
