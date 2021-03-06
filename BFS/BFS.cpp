#include <iostream>
#include <vector>
#include <queue>

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
 * breadth first search
 *
 * @param adj adjacency list of graph
 * @param start vertex from where DFS starts traversing
 *
 */
void BFS(vector<int> adj[], int start) {
    vector<bool> visited(adj->size(), false); //creates vector with size = number of vertices
    queue<int> queue; //create a queue for BFS

    queue.push(start);
    visited[start] = true;

    int curVertex, adjVertex;
    while (!queue.empty()) {
        curVertex = queue.front();
        queue.pop();
        cout << curVertex << " ";

        for (int i=0; i<adj[curVertex].size(); ++i) {
            adjVertex = adj[curVertex][i];
            if(!visited[adjVertex]) {
                queue.push(adjVertex);
                visited[adjVertex] = true;
            }
        }
    }
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
    addEdge(adj, 6, 10);
    addEdge(adj, 8, 10);

    BFS(adj, 0);

    return 0;
}
