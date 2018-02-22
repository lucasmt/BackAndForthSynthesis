#pragma once

#include "Graph.hpp"
#include "quick-cliques/src/Algorithm.h"

#include <list>
#include <functional>

class MFSGenerator
{
  Algorithm* pAlgorithm;
  char** adjacencyMatrix;
  int n;

public:
  MFSGenerator(const Graph& graph,
	       std::function<bool(const std::list<int>&)> callback);

  long run();

  ~MFSGenerator();
};
