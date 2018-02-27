#include "MFSGenerator.hpp"
#include "quick-cliques/src/TomitaAlgorithm.h"

#include <algorithm>

using std::function;
using std::list;
using std::vector;
using std::max;

MFSGenerator::MFSGenerator(const Graph& graph,
			   function<bool(const list<int>&)> callback)
{
  const vector<vector<int>>& vAdjacencyMatrix = graph.adjacencyMatrix();
  n = graph.size();

  adjacencyMatrix = (char**)calloc(n, sizeof(char*));

  for (int i = 0; i < n; i++)
  {
    adjacencyMatrix[i] = (char*)calloc(n, sizeof(char));

    for (int j = 0; j < n; j++)
      adjacencyMatrix[i][j] = vAdjacencyMatrix[i][j];
  }

  pAlgorithm = new TomitaAlgorithm(adjacencyMatrix, n);

  pAlgorithm->AddCallBack(callback);
}

long MFSGenerator::run()
{
  list<list<int>> cliques;

  return pAlgorithm->Run(cliques);
}

MFSGenerator::~MFSGenerator()
{
  if (adjacencyMatrix != nullptr)
  {
    int i = 0;

    while (i < n)
    {
      free(adjacencyMatrix[i]);
      i++;
    }

    free(adjacencyMatrix);
  }

  delete pAlgorithm;
}
