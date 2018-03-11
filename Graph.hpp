#pragma once

#include <tuple>

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

  Graph(const Set<V>& vertices); /**< constructs a graph with the given vertex set and no edges */
  Graph(const Vector<V>& vertices); /**< same as above */

  size_t size() const; /**< number of vertices in the graph */
  size_t edgeCount() const; /**< number of edges in the graph */

  V vertexByIndex(size_t i) const; /**< returns vertex corresponding to the given index */

  void addEdge(V from, V to); /**< adds and edge between the given vertices */
  Vector<std::tuple<V, V>> edges() const; /**< returns a list of all edges in the graph */

  /**
   * Returns a graph where:
   * - The vertex set is the same as the original graph;
   * - There is an edge between two different vertices iff there was no such edge in the original graph;
   * - There are no self-loops.
   */
  Graph<V> complement() const;
  
  Vector<Vector<size_t>> adjacencyMatrix() const; /**< adjacency matrix for the graph */

  /**
   * Returns a graph over the given vertex set where:
   * - There is an edge between two vertices iff there was such an edge in the original graph.
   */
  Graph<V> subgraph(const Set<V>& vertices) const;

  /**
   * Returns a list of all connected components of the graph (each given as a set of vertices).
   */
  Vector<Set<V>> connectedComponents() const;
};
