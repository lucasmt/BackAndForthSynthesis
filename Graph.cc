#include "Graph.hpp"

#include <iostream>
#include <stack>

using std::vector;
using std::set;
using std::stack;
using std::cout;
using std::endl;

Graph::Graph(int size)
  : _size(size),
    _vertices(_size),
    _adjacencyMatrix(_size, vector<int>(_size, 0))
{
  for (int i = 0; i < _size; i++)
  {
    _vertices[i] = i;
    _indices[i] = i;
  }
}

Graph::Graph(const set<int>& vertices)
  : _size(vertices.size()),
    _vertices(vertices.begin(), vertices.end()),
    _adjacencyMatrix(_size, vector<int>(_size, 0))
{
  for (int i = 0; i < _size; i++)
  {
    _indices[_vertices[i]] = i;
  }
}

Graph::Graph(const vector<int>& vertices)
  : _size(vertices.size()),
    _vertices(vertices),
    _adjacencyMatrix(_size, vector<int>(_size, 0))
{
  for (int i = 0; i < _size; i++)
  {
    _indices[_vertices[i]] = i;
  }
}

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

int Graph::vertex(int i) const
{
  return _vertices[i];
}

void Graph::addEdge(int from, int to)
{
  int i = _indices.at(from);
  int j = _indices.at(to);

  _adjacencyMatrix[i][j] = 1;
}

Graph Graph::complement() const
{
  int n = size();
  Graph complement(_vertices);
  
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      if (i != j && _adjacencyMatrix[i][j] == 0)
        complement.addEdge(_vertices[i], _vertices[j]);
        
  return complement;
}

const std::vector<std::vector<int>>& Graph::adjacencyMatrix() const
{
  return _adjacencyMatrix;
}

void Graph::print() const
{
  for (size_t i = 0; i < _adjacencyMatrix.size(); i++)
  {
    cout << _vertices[i] << " --> ";

    for (size_t j = 0; j < _adjacencyMatrix[i].size(); j++)
    {
      if (_adjacencyMatrix[i][j]) cout << _vertices[j] << " ";
    }

    cout << endl;
  }
}

Graph Graph::subgraph(const set<int>& vertices) const
{
  Graph subgraph(vertices);

  for (int v1 : vertices)
  {
    for (int v2 : vertices)
    {
      int i = _indices.at(v1);
      int j = _indices.at(v2);

      if (_adjacencyMatrix[i][j])
	subgraph.addEdge(v1, v2);
    }
  }

  return subgraph;
}

vector<set<int>> Graph::connectedComponents() const
{
  int n = size();
  vector<set<int>> components;
  set<int> allVertices;

  for (int i = 0; i < n; i++)
    allVertices.insert(i);

  while (!allVertices.empty())
  {
    set<int> newVertices;
    stack<int> toVisit;

    auto it = allVertices.begin();
    int head = *it;
    toVisit.push(head);
    newVertices.insert(head);
    allVertices.erase(it);

    do
    {
      int v = toVisit.top();
      toVisit.pop();

      for (int u = 0; u < n; u++)
      {
	if (_adjacencyMatrix[v][u])
	{
	  auto it = newVertices.find(u);

	  if (it == newVertices.end())
	  {
	    toVisit.push(u);
	    newVertices.insert(it, u);
	    allVertices.erase(u);
	  }
	}
      }
    }
    while (!toVisit.empty());

    set<int> component;

    for (int i : newVertices)
      component.insert(_vertices[i]);

    components.push_back(component);
  }

  return components;
}

