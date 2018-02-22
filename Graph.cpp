#include "Graph.hpp"

using std::vector;

Graph::Graph(int nVertices)
  : _adjacencyMatrix(nVertices, vector<int>(nVertices, 0))
{ }

int Graph::size() const
{
  return _adjacencyMatrix.size();
}

int Graph::nEdges() const
{
  int n = size();
  int c = 0;
  
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      if (_adjacencyMatrix[i][j] == 1)
        c++;
        
  return c;
}

void Graph::addEdge(int from, int to)
{
  _adjacencyMatrix[from][to] = 1;
}

Graph Graph::complement() const
{
  int n = size();
  Graph complement(n);
  
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      if (i != j && _adjacencyMatrix[i][j] == 0)
        complement.addEdge(i, j);
        
  return complement;
}

const std::vector<std::vector<int>>& Graph::adjacencyMatrix() const
{
  return _adjacencyMatrix;
}
