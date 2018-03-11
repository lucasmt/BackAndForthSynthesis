#pragma once

#include <tuple>
#include <stack>
#include <numeric>

#include "Vector.hpp"
#include "Map.hpp"
#include "Set.hpp"

/**
 * Unweighted graph over vertex set of type V.
 */
template <class V>
class Graph
{
  Vector<V> _vertices; /**< vertices of the graph */
  Map<V, size_t> _indices; /**< _indices[v] = i <-> _vertices[i] = v */
  Vector<Vector<size_t>> _adjacencyList; /**< _adjacencyList[i] contains the indices of the neighbors of _vertices[i] */
  size_t _edgeCount; /**< number of edges in the graph */

public:

  Graph(const Set<V>& vertices) /**< constructs a graph with the given vertex set and no edges */
    : _vertices(vertices.begin(), vertices.end()),
      _adjacencyList(vertices.size()),
      _edgeCount(0)
  {
    for (size_t i = 0; i < vertices.size(); i++)
    {
      _indices[_vertices[i]] = i;
    }
  }
  
  Graph(const Vector<V>& vertices) /**< same as above */
    : _vertices(vertices.begin(), vertices.end()),
      _adjacencyList(vertices.size()),
      _edgeCount(0)
  {
    for (size_t i = 0; i < vertices.size(); i++)
    {
      _indices[_vertices[i]] = i;
    }
  }

  size_t size() const /**< number of vertices in the graph */
  {
    return _vertices.size();
  }
  
  size_t edgeCount() const /**< number of edges in the graph */
  {
    return _edgeCount;
  }

  V vertexByIndex(size_t i) const /**< returns vertex corresponding to the given index */
  {
    return _vertices[i];
  }

  void addEdge(V from, V to) /**< adds and edge between the given vertices */
  {
    size_t i = _indices.at(from);
    size_t j = _indices.at(to);

    _adjacencyList[i].push_back(j);
    _edgeCount++;
  }
  
  Vector<std::tuple<V, V>> edges() const /**< returns a list of all edges in the graph */
  {
    Vector<std::tuple<V, V>> edges;

    for (size_t i = 0; i < _vertices.size(); i++)
      for (size_t j : _adjacencyList[i])
        edges.emplace_back(_vertices[i], _vertices[j]);

    return edges;
  }

  /**
   * Returns a graph where:
   * - The vertex set is the same as the original graph;
   * - There is an edge between two different vertices iff there was no such edge in the original graph;
   * - There are no self-loops.
   */
  Graph<V> complement() const
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
  
  Vector<Vector<size_t>> adjacencyMatrix() const /**< adjacency matrix for the graph */
  {
    size_t n = _vertices.size();
    Vector<Vector<size_t>> adjacencyMatrix(n, Vector<size_t>(n, 0));

    for (size_t i = 0; i < n; i++)
      for (size_t j : _adjacencyList[i])
        adjacencyMatrix[i][j] = 1;

    return adjacencyMatrix;
  }

  /**
   * Returns a graph over the given vertex set where:
   * - There is an edge between two vertices iff there was such an edge in the original graph.
   */
  Graph<V> subgraph(const Set<V>& vertices) const
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

  /**
   * Returns a list of all connected components of the graph (each given as a set of vertices).
   */
  Vector<Set<V>> connectedComponents() const
  {
    size_t n = _vertices.size();
    Vector<Set<V>> components;

    Vector<size_t> range(n);
    std::iota(range.begin(), range.end(), 0); /*< fill range with all numbers from 0 to n - 1 */
    Set<size_t> allIndices(range); /*< initialize set of all indices with { 0, ..., n - 1 } */

    /* Perform depth-first search */
    while (!allIndices.empty()) /*< stop when all indices have been added to some component */
    {
      Set<size_t> componentIndices; /*< indices forming a connected component */
      std::stack<size_t> toVisit; /*< indices that should be visited next */

      /* Start with first index that has not been added to a component yet */
      auto it = allIndices.begin();
      size_t head = *it;
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
	  auto it = componentIndices.find(j);

	  if (it == componentIndices.end())
	  {
	    toVisit.push(j);
	    componentIndices.insert(it, j);
	    allIndices.erase(j);
	  }
        }
      }
      while (!toVisit.empty());

      /* Construct a connected component from the indices */
      Set<V> component;

      for (size_t i : componentIndices)
        component.insert(_vertices[i]);

      components.push_back(component);
    }

    return components;
  }
};
