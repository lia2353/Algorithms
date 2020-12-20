#include <iostream>
#include <algorithm>
#include <cmath>
#include <map>
#include <sstream>
#include <stack>
#include <queue>
#include <unordered_set>
#include <vector>
#include <limits.h>
#include <chrono>

#define ROOT -1
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

#define FOUND -1

using namespace std;

//int memory = 0; check for memory leak
int N;               //N+1 tiles
int boardSize;       //the board is (boardSize x boardSize)

vector<int> goalState;
int goalBlankSpaceIndex;

class Node;
unordered_set<string> visited;
stack<pair<Node*,int>> path;            //Node and Direction
vector<pair<Node*,int>> neighbourNodes; //Node and Direction


class Node {
private:
    vector<int> currentState;      //board of N+1 tiles
    int currBlankSpaceIndex;       //the index of blank tile in the current board
    Node* parent;
    int manhattanDist;

public:
    Node(const vector<int>& currState, const int& currBlankSpace, Node* parent){
        currentState = vector<int>(currState);
        currBlankSpaceIndex = currBlankSpace;
        this->parent = parent;
        distManhattan();
        //memory++;  check for memory leak
    }

    Node(const vector<int>& currState, const int& currBlankSpace, Node* parent, const int& dist){
        currentState = vector<int>(currState);
        currBlankSpaceIndex = currBlankSpace;
        this->parent = parent;
        manhattanDist = dist;
        //memory++;  check for memory leak
    }

    ~Node() {
        //memory--;  check for memory leak
    }

    //Heuristics logic
    void distManhattan(){
        int dist = 0;
        for(int i = 0; i <= N; ++i){
            int currNum = currentState[i];
            int expectedNum = goalState[i];
            if(currNum != 0 && currNum != expectedNum) {
                //when blank space is after this number
                if(currNum == goalState[currNum-1]) {
                    dist += abs((i/boardSize) - ((currNum-1)/boardSize)) + abs((i%boardSize) - ((currNum-1)%boardSize));
                }
                //when blank space is before this number
                if(currNum == goalState[currNum]) {
                    dist += abs((i/boardSize) - (currNum/boardSize)) + abs((i%boardSize) - (currNum%boardSize));
                }
            }
            ++expectedNum;
        }
        manhattanDist = dist;
    }

    int distHamming() const{
        int conflicts = 0;
        for(int i=0; i<= N; ++i) {
            if(currentState[i] != 0 && currentState[i] != goalState[i]) {
                ++conflicts;
            }
        }
        return conflicts;
    }

    int getManhattanDist() const {
        return manhattanDist;
    }

    bool isGoal() const{
        return (this->manhattanDist == 0);
    }


    string toString() const {
        stringstream  result;
        for(int num : currentState) {
            result << num;
            result << ',';
        }
        return result.str();
    }


    Node* createNeighbourNode(const int& index) {
        //calculate Manhattan dist (change comes only for slided tile)
        int oldDist = 0, newDist = 0;
        int currNum = this->currentState[index];

        //when blank space is after this number
        if(currNum == goalState[currNum-1]) {
            oldDist = abs((index/boardSize) - ((currNum-1)/boardSize)) + abs((index%boardSize) - ((currNum-1)%boardSize));
            newDist = abs((currBlankSpaceIndex/boardSize) - ((currNum-1)/boardSize)) + abs((currBlankSpaceIndex%boardSize) - ((currNum-1)%boardSize));
        }
        //when blank space is before this number
        if(currNum == goalState[currNum]) {
            oldDist = abs((index/boardSize) - (currNum/boardSize)) + abs((index%boardSize) - (currNum%boardSize));
            newDist = abs((currBlankSpaceIndex/boardSize) - (currNum/boardSize)) + abs((currBlankSpaceIndex%boardSize) - (currNum%boardSize));
        }
        int dist = this->getManhattanDist() + newDist - oldDist;

        swap(currentState[currBlankSpaceIndex], currentState[index]);
        Node* neighbour = new Node(this->currentState, index, this, dist);
        //swap back the tiles for the current state
        swap(currentState[index], currentState[currBlankSpaceIndex]);

        return neighbour;
    }

    //create neighbour node with the direction that we made to come to it
    void neighbourNodesWithDirections() {
        neighbourNodes.clear();
        pair<int,int> blankPosition = {currBlankSpaceIndex / boardSize, currBlankSpaceIndex % boardSize};

        Node* neighbour;
        int direction;
        if(blankPosition.second + 1 < boardSize) {
            direction = LEFT;
            neighbour = this->createNeighbourNode(boardSize * blankPosition.first + blankPosition.second + 1);
            if (visited.find(neighbour->toString()) == visited.end()) {
                neighbourNodes.push_back(make_pair(neighbour, direction));
            } else {
               delete neighbour;
            }
        }
        if(blankPosition.second - 1 >= 0) {
            direction = RIGHT;
            neighbour = createNeighbourNode( boardSize * blankPosition.first + blankPosition.second - 1);
            if (visited.find(neighbour->toString()) == visited.end()) {
                neighbourNodes.push_back(make_pair(neighbour, direction));
            } else {
                delete neighbour;
            }
        }
        if(blankPosition.first + 1 < boardSize) {
            direction = UP;
            neighbour = createNeighbourNode( boardSize * (blankPosition.first + 1) + blankPosition.second);
            if (visited.find(neighbour->toString()) == visited.end()) {
                neighbourNodes.push_back(make_pair(neighbour, direction));
            } else {
                delete neighbour;
            }
        }
        if(blankPosition.first - 1 >= 0) {
            direction = DOWN;
            neighbour = createNeighbourNode( boardSize * (blankPosition.first - 1) + blankPosition.second);
            if (visited.find(neighbour->toString()) == visited.end()) {
                neighbourNodes.push_back(make_pair(neighbour, direction));
            } else {
                delete neighbour;
            }
        }
    }


    //Is the board solvable logic
    int inversionsCount() const{
		int count = 0;
		for(int i = 0; i <= N; ++i) {
			for(int j = i + 1; j <= N; ++j) {
				if(currentState[i] != 0 && currentState[j] != 0 && currentState[i] > currentState[j]) {
					++count;
				}
			}
		}
		return count;
	}

	bool isSolvable() {
        int inversionsCount = this->inversionsCount();

        bool isBoardSizeOdd = boardSize & 1;            //num AND 1 = 1 -> oddNum; num AND 1 == 0 -> evenNum
        //odd-sized board & even-inversions -> board is solvable
        if (isBoardSizeOdd && !(inversionsCount & 1))
            return true;
        //even-sized board & odd-sum (inversions + the row of the blank space) -> board is solvable
        else if(!isBoardSizeOdd && ((inversionsCount + (currBlankSpaceIndex / boardSize)) & 1))
            return true;
        return false;
    }
};


void printPath(stack<int>& path) {
     while (!path.empty()) {
        switch (path.top()){
            case LEFT : cout << "left\n"; break;
            case RIGHT : cout << "right\n"; break;
            case UP : cout << "up\n"; break;
            case DOWN : cout << "down\n"; break;
        }
        path.pop();
    }
}

//Limited DFS with given threshold
int LDFS(int moves, const int& threshold) {
    Node* currentNode = path.top().first;
    visited.insert(currentNode->toString());

    int cost = moves + currentNode->getManhattanDist();

    if(cost > threshold) {return cost;}
    if(currentNode->isGoal()) {return FOUND;}

    int minThreshold = INT_MAX;
    int result;

    currentNode->neighbourNodesWithDirections();
    for(pair<Node*,int> neighbour : (vector<pair<Node*,int>>)neighbourNodes ) {
        Node* node = neighbour.first;
        if(visited.find(node->toString()) == visited.end()) {
            path.push(make_pair(node, neighbour.second));

            result = LDFS(moves+1, threshold);
            visited.erase(node->toString());

            if(result == FOUND) {
                delete node;
                return FOUND;
            }

            cost = moves + 1 + node->getManhattanDist();
            if(result < minThreshold) {minThreshold = result;}
            path.pop();
        }
        delete node;
    }
    return minThreshold;
}

void IDAstar(Node* start) {
    int threshold = start->getManhattanDist();
    path.push({start, ROOT});

    int result = INT_MAX;
    while(result >= 0) {
        result = LDFS(0, threshold);
        visited.erase(start->toString());

        if(result == FOUND) {
            printf("%d\n", threshold);
            stack<int> pathDirections;
            while(!path.empty()) {
                pathDirections.push(path.top().second);
                path.pop();
            }
            printPath(pathDirections);
            return;
        }
        threshold = result;
    }
}



int main(){
    freopen("in.txt", "r", stdin);
	//freopen("out.txt", "w", stdout);

	//N-puzzle
	cin >> N;
	boardSize = sqrt(N+1);

    cin >> goalBlankSpaceIndex;
    if(goalBlankSpaceIndex == -1) goalBlankSpaceIndex = N;

    //Read start state
    vector<int> startState;
    int num, blankSpace = 0;
    for(int i = 0; i <= N; ++i) {
      cin >> num;
      startState.push_back(num);
      if(num == 0) blankSpace = i;
    }

    //Goal state
    num = 1;
    for(int i = 0; i <= N; ++i) {
        if(i == goalBlankSpaceIndex) {
            goalState.push_back(0);
        } else {
            goalState.push_back(num++);
        }
    }

    auto startTime = chrono::steady_clock::now();

    Node* startNode = new Node(startState, blankSpace, nullptr);
    if(startNode->isSolvable()) {
        IDAstar(startNode);
    } else {
        cout << "This board is unsolvable!\n";
    }

    delete startNode;

    auto endTime = std::chrono::steady_clock::now();
    chrono::duration<double> workTime = endTime - startTime;
    cout << "Work time: " << workTime.count() << " seconds\n";

    return 0;
}




