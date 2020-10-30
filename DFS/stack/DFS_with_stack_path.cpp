#include <iostream>
#include <vector>
#include <stack>
#include <utility>

using namespace std;

/**
 * Add the edge (u,v) in undirected graph.
 *
 * @param adj graph
 * @param u vertex at the end of the edge to be added
 * @param v vertex at the end of the edge to be added
 *
 */
void addEdge(vector<int> adj[], int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
}

/**
 * depth first search algorithm implemented with stack
 *
 * @param adj adjacency list of graph
 * @param start vertex from where DFS starts traversing
 * @param end vertex is the final destination
 * @param path vector to store the vertices on the path from start to end vertex
 * @return true if there is path from start to end vertex
 *
 */
bool iterativeDFS(vector<int> adj[], int start, int end, vector<int>& path) {
    vector<bool> visited(adj->size(), false); //creates vector with size = number of vertices
    stack<pair<int, int>> stack;

    visited[start] = true;
    stack.push({start,0});
    //cout << start << ' ';

    int curVertex, adjIndex, adjVertex;
    while (!stack.empty()) {
        curVertex = stack.top().first;
        adjIndex = stack.top().second;

        if (curVertex == end) {
            while (!stack.empty()){
                path.push_back(stack.top().first);
                stack.pop();
            }
            return true;
        }

        if (adj[curVertex].size() > adjIndex) {
            ++stack.top().second;
            adjVertex = adj[curVertex][adjIndex];
            if (!visited[adjVertex]) {
                stack.push({adjVertex,0});
                //cout << adjVertex << ' ';
                visited[adjVertex] = true;
            }
        } else {
            stack.pop();
        }
    }
    return false;
}

int main() {
    int V = 12;
    vector<int> adj[V];

    addEdge(adj, 0, 1);
    addEdge(adj, 0, 2);
    addEdge(adj, 0, 3);
    addEdge(adj, 2, 4);
    addEdge(adj, 3, 5);
    addEdge(adj, 3, 6);
    addEdge(adj, 5, 7);
    addEdge(adj, 5, 8);
    addEdge(adj, 5, 9);
    addEdge(adj, 6, 9);
    //addEdge(adj, 6, 10);
    addEdge(adj, 8, 10);

    //int start = 1, end = 11;   //no path
    int start = 0, end = 9;   //path exists

    vector<int> path; //creates vector to store the vertices from the path from start to end vertex

    bool pathExists = iterativeDFS(adj, start, end, path);
    cout << "There " << (pathExists ? "is " : "is no ") << "path between "
         << start << " and " << end << " vertices.\n";

    if(pathExists) {
        cout << "Path: ";
        for(int i=path.size()-1; i>=0; --i) {
            cout << path[i] << " ";
        }
    }


    return 0;
}
