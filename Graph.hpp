#pragma once

#include <vector>

class Graph
{
  std::vector<std::vector<int>> _adjacencyMatrix;

public:

  Graph(int nVertices);

  int size() const;
  
  int nEdges() const;

  void addEdge(int from, int to);

  Graph complement() const;

  const std::vector<std::vector<int>>& adjacencyMatrix() const;
};
