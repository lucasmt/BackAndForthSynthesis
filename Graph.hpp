#pragma once

#include <vector>
#include <set>
#include <unordered_map>

class Graph
{
  int _size;
  std::vector<int> _vertices;
  std::unordered_map<int, int> _indices;
  std::vector<std::vector<int>> _adjacencyMatrix;

public:

  Graph(int size);
  Graph (const std::set<int>& vertices);
  Graph (const std::vector<int>& vertices);

  int size() const;
  
  int nEdges() const;

  int vertex(int i) const;

  void addEdge(int from, int to);

  Graph complement() const;

  const std::vector<std::vector<int>>& adjacencyMatrix() const;

  void print() const;

  Graph subgraph(const std::set<int>& vertices) const;

  std::vector<std::set<int>> connectedComponents() const;
};
