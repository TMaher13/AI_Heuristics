/*
  Uninformed search algorithms on a graph

  Algorithms include:
    - Breadth first search
    - Uniform Cost search
    - Depth first search (option of limited-depth)

  Author: Thomas Maher
  Updated: 12/5/2019
*/
#include "Graph.hpp"
#include <string>

int main(int argc, char** argv) {
  /*
    Compile and give one command line argument with with uninformed search
      algorithm you would like to use. Options are:
        - BFS (Breadth-First Search)
        - DFS (Depth-First Search)
        - UCS (Uniform-Cost Search)
  */
  if(argc != 2) {
    std::cout << "Usage: ./main.exe <Search Algorithm>\n";
    std::cout << "BFS: Breadth-First Search\nDFS: Depth-First Search\nUCS: Uniform-Cost Search";
    return 0;
  }
  Graph g(5);
  g.addEdge(0, 2, 5);
  g.addEdge(0, 1, 1);
  g.addEdge(1, 2, 1);
  g.addEdge(2, 0, 1);
  g.addEdge(2, 3, 1);
  g.addEdge(2, 4, 5);
  g.addEdge(3, 4, 1);

  std::list<int> solution;
  if(std::string(argv[1]) == "BFS" || std::string(argv[1]) == "bfs")
    solution = g.BFS(4);
  else if(std::string(argv[1]) == "DFS" || std::string(argv[1]) == "dfs")
    solution = g.DFS(4);
  else if(std::string(argv[1]) == "UCS" || std::string(argv[1]) == "ucs")
    solution = g.UCS(4);

  std::cout << argv[1] << " solution:\n";
  for(auto & node : solution)
    std::cout << node << " ";

  return 0;
}
