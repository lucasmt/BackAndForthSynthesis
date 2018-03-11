#include "MFSGenerator.hpp"
#include "quick-cliques/src/TomitaAlgorithm.h"

using std::function;
using std::list;

MFSGenerator::MFSGenerator(const Graph<size_t>& graph,
			   function<bool(const list<int>&)> callback)
{
  Vector<Vector<size_t>> vAdjacencyMatrix = graph.adjacencyMatrix();
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
