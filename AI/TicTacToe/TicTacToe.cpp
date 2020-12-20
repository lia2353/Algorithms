#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <limits.h>

using namespace std;

#define BOARD_SIZE 3
#define EMPTY_SPACE '-'
#define FIRST_PLAYER 'X'
#define SECOND_PLAYER 'O'
char PLAYER_MAX,  PLAYER_MIN;
string PLAYER_MAX_WIN_CONFIG,  PLAYER_MIN_WIN_CONFIG;

#define HUMAN 0
#define COMPUTER 1

#define WIN 1
#define DRAW 0
#define LOSS -1

class Board {
    char board[BOARD_SIZE][BOARD_SIZE]; //= {{'O', EMPTY_SPACE, 'X'}, {'X', EMPTY_SPACE, EMPTY_SPACE}, {'X', 'O', 'O'}}; tests for optimality (check the picture)

public:
    Board() { }

    void initEmptyBoard() {
        for(int row = 0; row < BOARD_SIZE; ++row) {
            for(int col = 0; col < BOARD_SIZE; ++col) {
               board[row][col] = EMPTY_SPACE;
            }
        }
    }

    void print() const {
        for(int row = 0; row < BOARD_SIZE; ++row) {
            for(int col = 0; col < BOARD_SIZE; ++ col) {
                printf(" %c %s", board[row][col], (col == BOARD_SIZE -1) ? "" : "|");
            }
            printf("%s\n", (row == BOARD_SIZE-1) ? "" : "\n-----------");
        }
        printf("\n");
    }

    bool isEmptyPosition(const int& row, const int& col) const {
        return board[row][col] == EMPTY_SPACE;
    }

    bool hasNoEmptyPasitions() const {
         for(int row = 0; row < BOARD_SIZE; ++row) {
            for(int col = 0; col < BOARD_SIZE; ++ col) {
                if(isEmptyPosition(row, col)) {
                    return false;
                }
            }
         }
         return true;
    }

    void setValueTo(const char& value, const int& row, const int& col) {
        board[row][col] = value;
    }

    string getRow(const int& row) const {
        return string() + board[row][0] + board[row][1] + board[row][2];
    }

    string getCol(const int& col) const {
        return string() + board[0][col] + board[1][col] + board[2][col];
    }

    string getMainDiagonal() const {
        return string() + board[0][0] + board[1][1] + board[2][2];
    }

    string getSecondaryDiagonal() const {
        return string() + board[0][2] + board[1][1] + board[2][0];
    }

} gameBoard;

// Minimax algorithm with Alpha–beta pruning
bool isTerminalState(const Board& board, int& boardState) {
    string mainDiagonal = board.getMainDiagonal();
    string secondaryDiagonal = board.getSecondaryDiagonal();
    if(mainDiagonal == PLAYER_MAX_WIN_CONFIG || secondaryDiagonal == PLAYER_MAX_WIN_CONFIG) {
        boardState = WIN;
        return true;
    }
    if(mainDiagonal == PLAYER_MIN_WIN_CONFIG || secondaryDiagonal == PLAYER_MIN_WIN_CONFIG) {
        boardState = LOSS;
        return true;
    }

    for(int i = 0; i < BOARD_SIZE; ++i) {
        string row = board.getRow(i);
        string col = board.getCol(i);
        if(row == PLAYER_MAX_WIN_CONFIG || col == PLAYER_MAX_WIN_CONFIG) {
            boardState = WIN;
            return true;
        }
        if(row == PLAYER_MIN_WIN_CONFIG || col == PLAYER_MIN_WIN_CONFIG) {
            boardState = LOSS;
            return true;
        }
    }

   if(board.hasNoEmptyPasitions()) {
        boardState = DRAW;
        return true;
   }

   return false;
}

void getChildren(const Board& board, const int& player, vector<Board>& children) {
    for(int row = 0; row < BOARD_SIZE; ++row) {
        for(int col = 0; col < BOARD_SIZE; ++col) {
            if(board.isEmptyPosition(row, col)) {
                Board child(board);
                child.setValueTo(player, row, col);
                children.push_back(child);
            }
        }
    }
}

int minimax(const Board& board, bool isMaxPlayerTurn, int alpha, int beta, int &depth) {
    // If board is a terminal state (leaf node)
    int boardState;
    if (isTerminalState(board, boardState)) {
        return boardState;
    }

    ++depth;

    if(isMaxPlayerTurn) {
        int bestValue = INT_MIN;
        vector<Board> children;
        children.reserve(BOARD_SIZE * BOARD_SIZE);
        getChildren(board, PLAYER_MAX, children);

        for (auto child : children) {
            bestValue = max(bestValue, minimax(child, false, alpha,  beta,  depth));
            alpha = max(alpha, bestValue);
            if (alpha >= beta) {
                return bestValue;
            }
        }
        return bestValue;
    }
    //It's Min Player turn
    else  {
        int bestValue = INT_MAX;
        vector<Board> children;
        children.reserve(BOARD_SIZE * BOARD_SIZE);
        getChildren(board, PLAYER_MIN, children);

        for (auto child : children) {
            bestValue = min(bestValue, minimax(child, true, alpha, beta, depth));
            beta = min(beta, bestValue);
            if (beta <= alpha) {
                return bestValue;
            }
        }
        return bestValue;
    }
}

void makeOptimalMove(Board& board) {
    int bestValue = LOSS;
    int bestDepth = INT_MAX;
    Board bestBoard;

    vector<Board> children;
    children.reserve(BOARD_SIZE * BOARD_SIZE);
    getChildren(gameBoard, PLAYER_MAX, children);

    for (auto child : children) {
        int depth = 0;
        int value = minimax(child, false, INT_MIN, INT_MAX, depth);

        if(value > bestValue) {
            bestValue = value;
            board = child;
            bestDepth = depth;
        } else if(value == bestValue && depth < bestDepth){
            board = child;
            bestDepth = depth;
        }
    }
}

class TicTacToe {
    bool firstPlayer;

public:
    TicTacToe() {
        printf("********************\n  Tic-Tac-Toe Game\n********************\n");

        //Choose who is plays first
        printf("\nDo you want to play first? [y/n] ");
        char c;
        scanf("%c", &c);
        firstPlayer = (c == 'y') ? HUMAN : COMPUTER;

        //First player always plays with 'X' and second player always plays with 'O'
        //MAX is the COMPUTER and MIN is the HUMAN
        PLAYER_MAX = (firstPlayer == HUMAN) ? SECOND_PLAYER : FIRST_PLAYER;
        PLAYER_MIN = (firstPlayer == HUMAN) ? FIRST_PLAYER : SECOND_PLAYER;
        PLAYER_MAX_WIN_CONFIG = string() + PLAYER_MAX + PLAYER_MAX + PLAYER_MAX;
        PLAYER_MIN_WIN_CONFIG = string() + PLAYER_MIN + PLAYER_MIN + PLAYER_MIN;

        gameBoard.initEmptyBoard();
    }

    void humanMakeMove() {
        int row, col;
        scanf("%d %d", &row, &col);

        while(row < 1 || row > BOARD_SIZE || col < 1 || col > BOARD_SIZE || !gameBoard.isEmptyPosition(row - 1, col - 1) ) {
            printf("Incorrect move. \nShould be empty (row, col) where row and col are numbers in [1,3].\n");
            scanf("%d %d", &row, &col);
        }

        //MIN is the HUMAN
        gameBoard.setValueTo(PLAYER_MIN, row - 1, col - 1);
    }

    void printResult(int resultOfTheGame) {
        if(resultOfTheGame == WIN) {
            printf("Computer (player Max)  has won the game!");
        } else if(resultOfTheGame == LOSS) {
            printf("Human (player Min) has won the game!");
        } else {
            printf("Draw");
        }
    }

    void play() {
        if (firstPlayer == HUMAN) {
            printf("\nBoard: \n");
            gameBoard.print();
            humanMakeMove();
            gameBoard.print();
        }

        int resultOfTheGame;
        while ( !isTerminalState(gameBoard, resultOfTheGame)) {
            makeOptimalMove(gameBoard);
            printf("Computer: \n");
            gameBoard.print();
            if (isTerminalState(gameBoard, resultOfTheGame)) {
                    break;
            }
            humanMakeMove();
            gameBoard.print();
        }

        printResult(resultOfTheGame);
    }

};

int main() {
    TicTacToe game;
    game.play();

    return 0;
}

