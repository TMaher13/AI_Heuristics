/*
  Header file containing graph used for traversal by uninformed search algorithms
*/

#ifndef __GRAPH__
#define __GRAPH__

#include <iostream>
#include <list>
#include <utility>


class Graph {
  private:
    int N; // Number of nodes in graph
    int start; // Starting position, defaults to node 0
    std::list<std::pair<int,int>>* adj; // Pointer to array of adjacency lists containing (node, weight)

  public:
    Graph(int n) : N(n) {
      start = 0;
      adj = new std::list<std::pair<int,int>>[N];
    };

    Graph(int n, int initPos) : N(n), start(initPos) {
      adj = new std::list<std::pair<int,int>>[N];
    };

    int getStart() {
      return start;
    };

    int getNumNodes() {
      return N;
    };

    std::list<std::pair<int,int>> getAdjacent(int n) {
      return adj[n];
    };

    // Add edge of from node n1 to node n2 with weight w
    void addEdge(int n1, int n2, int w) {
      adj[n1].push_back(std::pair<int,int>(n2,w));
    };

    // Breadth-First Search from start node to goal node
    std::list<int> BFS(int goal);

    // Depth-First Search from start node to goal node
    std::list<int> DFS(int goal, int l=NULL);
    std::list<int> DFSRecursive(int currNode, int goal, int l=NULL);

    // Uniform Cost Search from start node to goal
    std::list<int> UCS(int goal);
};

#include "Graph.cpp"
#endif
