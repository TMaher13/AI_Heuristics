#include <algorithm>
#include <list>
#include <deque>
#include <unordered_map>

std::list<int> Graph::BFS(int goal) {
  /*
    Breadth-First Search on a graph state-space. This uninformed search algorithm is complete
      and guarenteed to find the shortest path to the goal state. Large time and space complexities.

    @param goal: The goal node we want to find the path to

    @return: List containing the shortest path of nodes to goal state
  */
  if(getStart() == goal) {
    std::cout << "Starting position is the destination.";
    return {};
  }
  std::deque<int> frontier;
  std::list<int> explored, solution;
  std::unordered_map<int,int> map;
  frontier.push_back(start);
  int position;

  while(!frontier.empty()) {
    position = frontier.front(); frontier.pop_front();
    explored.push_back(position);
    for(auto & child : getAdjacent(position)) {
      auto it_ex = std::find(explored.begin(), explored.end(), child.first);
      auto it_fr = std::find(frontier.begin(), frontier.end(), child.first);
      if(it_ex == explored.end() && it_fr == frontier.end()) {
        map[child.first] = position;
        if(child.first == goal) {
          int path = child.first;
          solution.push_back(child.first);
          while(path != getStart()) {
            path = map[path];
            solution.push_front(path);
          }
          return solution;
        }
        frontier.push_back(child.first);
      }
    }
  }

  return {};
}

std::list<int> Graph::DFS(int goal, int l) {
  /*
    Depth-First Search on a graph state-space. This uninformed search algorithm is not complete,
      and is not guarenteed to find a solution, and does not guarentee solution is optimal. However,
      space requirements are extremely small compared to BFS.

    @param goal: The goal node we want to find the path to
    @param l: Optional parameter defining the depth limit for DFS

    @return: List containing path of nodes to goal at lowest cost
  */
  if(getStart() == goal) {
    std::cout << "Starting position is the destination.";
    return {};
  }

  if(l == NULL)
    return DFSRecursive(0, goal, getNumNodes());
  else
    return DFSRecursive(0, goal, l);
}

std::list<int> Graph::DFSRecursive(int currNode, int goal, int l) {
  /*
    Recursive function used for DFS. Called originally from DFS and returns an solution, or
      an empty list if no solution is found.

    @param currNode: The current node the search agent is located at
    @param goal: The goal node we want to find the path to
    @param l: ptional parameter defining the depth limit for DFS

    @return: List containing the internal path for recursive call
  */
  if(currNode == goal)
    return {currNode};
  else if(l == 0)
    return {};
  else {
    for(auto & child : getAdjacent(currNode)) {
      std::list<int> result = DFSRecursive(child.first, goal, l-1);
      if(result.size() != 0) { // If solution was found
        if(result.front() != getStart()) // Make sure we don't add repetitive loop to solution
          result.push_front(currNode);
        return result;
      }
    }
  }
  return {};
}

bool lowerCost(const std::pair<int,int> &lhs, const std::pair<int,int> &rhs) {
  /*
    Used for finding the lowest-cost path in the frontier for Uniform-Cost Search

    @param lhs: The left-hand object to compare
    @ param rhs: The right-hand object to compare

    @return: If the left object's cost is less than the right object's cost
  */
  return lhs.second < rhs.second;
}

std::list<int> Graph::UCS(int goal) {
  /*
    Uniform-Cost Search on a graph state-space. This uninformed search algorithm
      finds the lowest costing path to the goal state.

    @param goal: The goal node we want to find the path to

    @return: List containing path of nodes to goal at lowest cost
  */
  if(getStart() == goal) {
    std::cout << "Starting position is the destination.";
    return {};
  }

  std::deque<std::pair<int,int>> frontier;
  frontier.push_back(std::pair<int,int>(getStart(),0));
  std::list<int> explored, solution;
  std::unordered_map<int,int> map;
  std::pair<int,int> position;

  while(!frontier.empty()) {
    auto minStep = std::min_element(frontier.begin(), frontier.end(), lowerCost);
    position = *minStep;
    frontier.erase(minStep);
    if(position.first == goal) {
      int path = position.first;
      solution.push_back(position.first);
      while(path != getStart()) {
        path = map[path];
        solution.push_front(path);
      }
      return solution;
    }
    explored.push_back(position.first);
    for(auto & child : getAdjacent(position.first)) {
      auto it_ex = std::find(explored.begin(), explored.end(), child.first);
      auto it_fr = std::find(frontier.begin(), frontier.end(), child);
      if(it_ex == explored.end() && it_fr == frontier.end()) {
        map[child.first] = position.first;
        frontier.push_back(child);
      }
      else if(it_fr != frontier.end() && it_fr->second > child.second) {
        it_fr->second = child.second;
      }
    }

  }

  return {};
}
