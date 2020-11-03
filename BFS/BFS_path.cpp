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
bool BFS(vector<int> adj[], int V, int start, int end, vector<int>& path) {
    vector<bool> visited(adj->size(), false); //creates vector with size = number of vertices
    queue<int> queue; //create a queue for BFS
    vector<int> parent(V);

    queue.push(start);
    visited[start] = true;
    parent[start] = start;

    int curVertex, adjVertex;
    while (!queue.empty()) {
        curVertex = queue.front();
        queue.pop();

        for (int i=0; i<adj[curVertex].size(); ++i) {
            adjVertex = adj[curVertex][i];
            if (!visited[adjVertex]) {
                queue.push(adjVertex);
                visited[adjVertex] = true;
                parent[adjVertex] = curVertex;

                if (adjVertex == end) {
                    //go back from end to start in parent array
                    curVertex = end;
                    path.push_back(end);
                    while (parent[curVertex] != start) {
                        curVertex = parent[curVertex];
                        path.push_back(curVertex);
                    }
                    path.push_back(start);
                    return true;
                }
            }
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
    addEdge(adj, 6, 10);
    addEdge(adj, 8, 10);

    int start = 0, end = 9;
    vector<int> path; //creates vector to store the vertices from the path from start to end vertex

    bool pathExists = BFS(adj, V, start, end, path);
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
