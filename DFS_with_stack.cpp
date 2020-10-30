//#include <iostream>
//#include <vector>
//#include <stack>
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
// * depth first search algorithm implemented with stack
// *
// * @param adj adjacency list of graph
// * @param start vertex from where DFS starts traversing
// *
// */
//void itarativeDFS(vector<int> adj[], int start) {
//    vector<bool> visited(adj->size(), false); //creates vector with size = number of vertices
//    stack<int> stack;
//
//    stack.push(start);
//    visited[start] = true;
//
//    int curVertex, adjVertex;
//    while (!stack.empty()) {
//        curVertex = stack.top();
//        stack.pop();
//        cout << curVertex << " ";
//
//        for (int i=adj[curVertex].size()-1; i>=0; --i) {
//            adjVertex = adj[curVertex][i];
//            if(!visited[adjVertex]) {
//                stack.push(adjVertex);
//                visited[adjVertex] = true;
//            }
//        }
//    }
//}
//
//int main() {
//    int V = 8;
//    vector<int> adj[V];
//
//    // Vertex numbers should be from 0 to 7
//    addEdge(adj, 0, 3);
//    addEdge(adj, 0, 5);
//    addEdge(adj, 0, 7);
//    addEdge(adj, 1, 4);
//    addEdge(adj, 1, 6);
//    addEdge(adj, 2, 6);
//    addEdge(adj, 4, 7);
//    addEdge(adj, 5, 6);
//    addEdge(adj, 5, 7);
//    addEdge(adj, 6, 7);
//
//    itarativeDFS(adj, 1);
//
//    return 0;
//}
