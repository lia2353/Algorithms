#include <iostream>
#include <vector>
#include <algorithm>
#include <limits.h>
#include <time.h>
#include <chrono>

using namespace std;

class Board {

public:
    vector<int> queens; // Every queens is on different column and we change only its row so we keep track only on witch row is the queen
    int boardSize;      // N

    vector<int> queensOnRow;            // Count of queens on each row
    vector<int> queensOnLeftDiagonal;   // Count of queens on each left diagonal (main diagonal)
    vector<int> queensOnRightDiagonal;  // Count of queens on each right diagonal (secondary diagonal)


    void init() {
        // Optimization: Every queen on different column
        for(int row = 0; row < boardSize; ++row) {
            queens[row] = row;
        }

        // Put queens on random rows
        random_shuffle(queens.begin(), queens.end());

        fill(queensOnRow.begin(), queensOnRow.end(), 0);
        fill(queensOnLeftDiagonal.begin(), queensOnLeftDiagonal.end(), 0);
        fill(queensOnRightDiagonal.begin(), queensOnRightDiagonal.end(), 0);
    }

    /* Returns the index of left diagonal on witch the given cell belongs
     * Diagonals' indexes: 0 to 2 * N - 1
     * Formula: (N - 1) - (y - x)
     *
     * Example for N = 3
     * y/x   0 1 2
     *  0   |2|3|4|
     *  1   |1|2|3|
     *  2   |0|1|2|
     * Diagonal index for (1,2) -> (3 - 1) - (2 - 1) = 3
    */
    int getLeftDiagonalIndex(int row, int col) {
        return (boardSize - 1) - (row - col);
    }

    /* Returns the index of right diagonal on witch the given cell belongs
     * Diagonals' indexes: 0 to 2 * N - 1
     * Formula: x + y
     *
     * Example for N = 3
     * y/x   0 1 2
     *  0   |0|1|2|
     *  1   |1|2|3|
     *  2   |2|3|4|
     * Diagonal index for (1,2) -> 1 + 2 = 3
    */
    int getRightDiagonalIndex(int row, int col) {
        return row + col;
    }

    // Increment number of queens on the row, left and right diagonal
    void calculateAllConflicts() {
        for(int col = 0; col < boardSize; ++col) {
            int qRow = queens[col];

            ++queensOnRow[qRow];
            ++queensOnLeftDiagonal[getLeftDiagonalIndex(qRow, col)];
            ++queensOnRightDiagonal[getRightDiagonalIndex(qRow, col)];
        }
    }

    bool hasConflicts() {
        for(int col = 0; col < boardSize; ++col) {
            if(getQueenConflictNumber(col) > 0) {
                return true;
            }
        }
        return false;
    }

    // Each  queen can have conflicts on its row, on its left diagonal or on its right diagonal
    int getQueenConflictNumber(int col) {
        int qRow = queens[col];

        // (number - 1) because we count the given queen (in conflict with itself)
        return queensOnRow[qRow] - 1 + queensOnLeftDiagonal[getLeftDiagonalIndex(qRow, col)] - 1
                                     + queensOnRightDiagonal[getRightDiagonalIndex(qRow, col)] - 1;
    }

    // Return the col with max conflicts (the queen with max conflicts)
    int getColWithQueenWithMaxConflicts() {
        int maxConflicts = 0;
        int currConflicts = 0;
        vector<int> candidates(boardSize);
        int candicatesCount = 0;

        for(int col = 0; col < boardSize; ++col) {
            currConflicts = getQueenConflictNumber(col);

            if(currConflicts > maxConflicts) {
                maxConflicts = currConflicts;
                candicatesCount = 0;
                candidates[candicatesCount++] = col;
            } else if(currConflicts == maxConflicts) {
                candidates[candicatesCount++] = col;
            }
        }

        if(candicatesCount > 1) {
            // If more than one candidate, choose randomly candidate
            int candidateIndex = rand() % candicatesCount;
            return candidates[candidateIndex];
        }
        return candidates[0];
    }

    // Return the row with min conflicts for given column
    int getRowWithMinConflicts(int col){
        int minConflicts = INT_MAX;
        int currConflicts = 0;
        vector<int> candidates(boardSize);
        int candidatesCount = 0;

        for(int row = 0; row < boardSize; ++row) {
            if(row != queens[col]) {
                currConflicts = queensOnRow[row] + queensOnLeftDiagonal[getLeftDiagonalIndex(row, col)]
                                             + queensOnRightDiagonal[getRightDiagonalIndex(row, col)];

                if(currConflicts < minConflicts) {
                    minConflicts = currConflicts;
                    candidatesCount = 0;
                    candidates[candidatesCount++] = row;
                } else if(currConflicts == minConflicts) {
                    candidates[candidatesCount++] = row;
                }
            }
        }

        if(candidatesCount > 1) {
            // If more than one candidate, choose randomly candidate
            int candidateIndex = rand() % candidatesCount;
            return candidates[candidateIndex];
        }
        return candidates[0];
    }

    void updateConflicts(int oldRow, int newRow, int col) {
        --queensOnRow[oldRow];
        ++queensOnRow[newRow];

        --queensOnLeftDiagonal[getLeftDiagonalIndex(oldRow, col)];
        ++queensOnLeftDiagonal[getLeftDiagonalIndex(newRow, col)];

        --queensOnRightDiagonal[getRightDiagonalIndex(oldRow, col)];
        ++queensOnRightDiagonal[getRightDiagonalIndex(newRow, col)];
    }

public:
    Board(int boardSize) {
        this->boardSize = boardSize;
        queens = vector<int>(boardSize);
        queensOnRow = vector<int>(boardSize);
        queensOnLeftDiagonal = vector<int>(2 * boardSize - 1); //2 * N -1 diagonals
        queensOnRightDiagonal = vector<int>(2 * boardSize- 1);

        srand(time(0));
    }

    //MinConflicts Algorithm
    void solve() {
        // Guaranties that if it stucks in a loop, after a given number of iterations it will restart
        int MAX_ITERATIONS = 3 * boardSize;

        init();
        calculateAllConflicts();

        for(int i = 0; i < MAX_ITERATIONS; ++i) {
            // Optimization: Pick queen with max conflicts (randomly if two or more) and put it on the row with min conflicts (randomly of two or more)
            int col = getColWithQueenWithMaxConflicts();
            int row = getRowWithMinConflicts(col);

            int oldRow = queens[col];

            queens[col] = row;
            updateConflicts(oldRow, row, col);

            if(!hasConflicts()) {
                return;
            }
        }

        if(hasConflicts()) {
            // Restart
            solve();
        }
    }

    void print() {
        for(int row = 0; row < boardSize; ++row) {
            for(int col = 0; col < boardSize; ++col) {
                if(queens[col] == row) {
                    printf("* ");
                } else {
                    printf("_ ");
                }
            }
            printf("\n");
        }
    }

};

int main()
{
	int N;
	printf("N = ");
	scanf("%d", &N);

	auto startTime = chrono::steady_clock::now();

	Board board(N);
	board.solve();

	auto endTime = chrono::steady_clock::now();
    chrono::duration<double> workTime = endTime - startTime;
    printf("Work time: %f seconds\n", workTime.count());

    if(N <= 20) {
        board.print();
    }
    return 0;
}
