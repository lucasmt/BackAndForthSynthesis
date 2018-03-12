#pragma once

#include "Graph.hpp"
#include "quick-cliques/src/Algorithm.h"

#include <list>
#include <functional>

/**
 * Class that generates Maximal Cliques representing Maximal Falsifiable Subsets.
 */
class MFSGenerator
{
  Algorithm* pAlgorithm; /**< algorithm used to generate maximal cliques */
  char** adjacencyMatrix; /**< adjacency matrix of the graph */
  int n; /**< number of vertices of the graph */

public:

  /** Constructs a generator that calls the callback function for every max-clique of the graph. */
  MFSGenerator(const Graph<size_t>& graph,
	       std::function<bool(const std::list<int>&)> callback);

  /** Run the maximal-clique generation procedure */
  long run();

  ~MFSGenerator();
};
