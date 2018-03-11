#include "Graph.hpp"

#include <stack>

using std::stack;

Graph::Graph(const Set<T>& vertices)
  : _vertices(vertices.begin(), vertices.end()),
    _adjacencyList(vertices.size()),
    _edgeCount(0)
{
  for (size_t i = 0; i < vertices.size(); i++)
  {
    _indices[_vertices[i]] = i;
  }
}

Graph::Graph(const Vector<T>& vertices)
  : _vertices(vertices.begin(), vertices.end()),
    _adjacencyList(vertices.size()),
    _edgeCount(0)
{
  for (size_t i = 0; i < vertices.size(); i++)
  {
    _indices[_vertices[i]] = i;
  }
}

size_t Graph::size() const
{
  return _vertices.size();
}

size_t Graph::edgeCount() const
{
  return _edgeCount;
}

size_t Graph::vertexByIndex(size_t i) const
{
  return _vertices[i];
}

void Graph::addEdge(V from, V to)
{
  size_t i = _indices.at(from);
  size_t j = _indices.at(to);

  _adjacencyList[i].push_back(j);
  _edgeCount++;
}

Graph<V> Graph::complement() const
{
  size_t n = _vertices.size();
  Graph<V> complement(_vertices);

  /* Add an edge (u, v) for every distinct u and v that are not neighbors in the original graph */
  for (size_t i = 0; i < n; i++)
  {
    Vector<bool> isNeighbor(n);

    for (size_t j : _adjacencyList[i])
      isNeighbor[j] = true;

    for (size_t j = 0; j < n; j++)
      if (i != j && !isNeighbor[j])
	complement.addEdge(_vertices[i], _vertices[j]);
  }

  return complement;
}

Vector<Vector<int>> Graph::adjacencyMatrix() const
{
  size_t n = _vertices.size();
  Vector<Vector<int>> adjacencyMatrix(n, Vector<int>(n, 0));

  for (size_t i = 0; i < n; i++)
    for (size_t j : _adjacencyList[i])
      adjacencyMatrix[i][j] = 1;

  return adjacencyMatrix;
}

Graph<V> Graph::subgraph(const Set<V>& vertices) const
{
  Graph<V> subgraph(vertices);

  /* For every vertex v1 in the subgraph... */
  for (V v1 : vertices)
  {
    size_t i = _indices.at(v1);

    /* For every neighbor v2 of v1 in the supergraph... */
    for (size_t j : _adjacencyList[i])
    {
      V v2 = _vertices[j];
      
      if (vertices.find(v2) != vertices.end()) /* If v2 is in the subgraph... */
	subgraph.addEdge(v1, v2); /*< ...add an edge from v1 to v2 */
    }
  }

  return subgraph;
}

Vector<Set<V>> Graph::connectedComponents() const
{
  size_t n = _vertices.size();
  Vector<Set<V>> components;

  Vector<size_t> range(n);
  iota(range.begin(), range.end(), 0); /*< fill range with all numbers from 0 to n - 1 */
  Set<size_t> allIndices(range); /*< initialize set of all indices with { 0, ..., n - 1 } */

  /* Perform depth-first search */
  while (!allIndices.empty()) /*< stop when all indices have been added to some component */
  {
    Set<size_t> componentIndices; /*< indices forming a connected component */
    stack<size_t> toVisit; /*< indices that should be visited next */

    /* Start with first index that has not been added to a component yet */
    size_t head = *allIndices.begin();
    toVisit.push(head);
    componentIndices.insert(head);
    allIndices.erase(it);

    /* Repeat until we have visited every vertex in this connected component */
    do
    {
      size_t i = toVisit.top(); /*< take first index from the stack */
      toVisit.pop();

      /* Add every neighbor that has not been visited yet to the stack */
      for (size_t j : _adjacencyList[i])
      {
	auto it = newIndices.find(j);

	if (it == newVertices.end())
	{
	  toVisit.push(j);
	  newIndices.insert(it, j);
	  allIndices.erase(j);
	}
      }
    }
    while (!toVisit.empty());

    /* Construct a connected component from the indices */
    Set<V> component;

    for (size_t i : newIndices)
      component.insert(_vertices[i]);

    components.push_back(component);
  }

  return components;
}

