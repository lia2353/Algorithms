#include <iostream>
#include <vector> //Vectors are sequence containers representing arrays that can change in size.

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
 * Recursive function for traversal of the vertices adjacent to v.
 *
 * @param curVertex vertex to be explored
 * @param adj graph
 * @param visited already visited vertices
 *
 */
void DFSuntil(int curVertex, vector<int> adj[], vector<bool> &visited) {
    cout << curVertex << " ";
     //mark the vertex as visited
    visited[curVertex] = true;

    int adjVertex;
    for (int i=0; i<adj[curVertex].size(); ++i) {
        adjVertex = adj[curVertex][i];
        if(!visited[adjVertex]) {
            DFSuntil(adjVertex, adj, visited);
        }
    }
}

/**
 * recursive depth first search algorithm.
 *
 * @param adj adjacency list of graph
 * @param start vertex from where DFS starts traversing
 *
 */
void recDFS(vector<int> adj[], int start) {
    vector<bool> visited(adj->size(), false); //creates vector with size = number of vertices
    DFSuntil(start, adj, visited);
}

int main() {
    int V = 8;
    vector<int> adj[V];

    // Vertex numbers should be from 0 to 7
    addEdge(adj, 0, 3);
    addEdge(adj, 0, 5);
    addEdge(adj, 0, 7);
    addEdge(adj, 1, 4);
    addEdge(adj, 1, 6);
    addEdge(adj, 2, 6);
    addEdge(adj, 4, 7);
    addEdge(adj, 5, 6);
    addEdge(adj, 5, 7);
    addEdge(adj, 6, 7);

    recDFS(adj, 1);

    return 0;
}
