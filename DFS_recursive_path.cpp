//#include <iostream>
//#include <vector>
//
//using namespace std;
//
///**
// * Add the edge (u,v) in undirected graph.
// *
// * @param adj graph
// * @param u vertex at the end of the edge to be added
// * @param v vertex at the end of the edge to be added
// *
// */
//void addEdge(vector<int> adj[], int u, int v) {
//    adj[u].push_back(v);
//    adj[v].push_back(u);
//}
//
///**
// * Recursive function for traversal of the vertices adjacent to v.
// *
// * @param curVertex vertex to be explored
// * @param end vertex is the final destination
// * @param adj graph
// * @param visited already visited vertices
// * @param path vector to store the vertices on the path
// * @param foundPath true if path from start to end is found
// *
// */
//bool DFSuntil(int curVertex, int end, vector<int> adj[], vector<bool>& visited, vector<int>& path, bool& foundPath) {
//    path.push_back(curVertex);
//    //mark the vertex as visited
//    visited[curVertex] = true;
//
//    if(curVertex == end) {
//        foundPath = true;
//        return true;
//    }
//
//    int adjVertex;
//    for (int i=0; i<adj[curVertex].size(); ++i) {
//        adjVertex = adj[curVertex][i];
//        if (!visited[adjVertex]) {
//            DFSuntil(adjVertex, end, adj, visited, path, foundPath);
//            //backtrack
//            if (foundPath) { return true; }
//            path.pop_back();
//        }
//    }
//
//}
//
///**
// * recursive depth first search algorithm.
// *
// * @param adj adjacency list of graph
// * @param start vertex from where DFS starts traversing
// * @param end vertex is the final destination
// * @param path vector to store the vertices on the path from start to end vertex
// * @return true if there is path from start to end vertex
// *
// */
//bool recDFS(vector<int> adj[], int start, int end, vector<int>& path) {
//    vector<bool> visited(adj->size(), false); //creates vector with size = number of vertices
//    bool foundPath = false;
//    DFSuntil(start, end, adj, visited, path, foundPath);
//    return foundPath;
//}
//
//int main() {
//    int V = 12;
//    vector<int> adj[V];
//
//    addEdge(adj, 0, 1);
//    addEdge(adj, 0, 2);
//    addEdge(adj, 0, 3);
//    addEdge(adj, 2, 4);
//    addEdge(adj, 3, 5);
//    addEdge(adj, 3, 6);
//    addEdge(adj, 5, 7);
//    addEdge(adj, 5, 8);
//    addEdge(adj, 5, 9);
//    addEdge(adj, 6, 9);
//    //addEdge(adj, 6, 10);
//    addEdge(adj, 8, 10);
//
//    //int start = 1, end = 11;   //no path
//    int start = 0, end = 9;   //path exists
//
//    vector<int> path; //creates vector to store the vertices from the path from start to end vertex
//
//
//    bool pathExists = recDFS(adj, start, end, path);
//    cout << "There " << (pathExists ? "is " : "is no ") << "path between "
//         << start << " and " << end << " vertices.\n";
//
//    if(pathExists) {
//        cout << "Path: ";
//        for(int i=0; i<path.size(); ++i) {
//            cout << path[i] << " ";
//        }
//    }
//
//
//    return 0;
//}
//
