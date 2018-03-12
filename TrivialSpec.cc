#include "TrivialSpec.hpp"
#include "Map.hpp"

#include <numeric>

using std::move;
using std::iota;
using std::function;

TrivialSpec::TrivialSpec(Vector<BVar> defined, Vector<CNFClause> negDefinitions)
  : _defined(move(defined)),
    _negDefinitions(move(negDefinitions))
{}

void TrivialSpec::forEach(function<void(BVar, const CNFClause&)> visitor) const
{
  for (size_t i = 0; i < _defined.size(); i++)
    visitor(_defined[i], _negDefinitions[i]);
}

Graph<size_t> TrivialSpec::conflictGraph() const
{
  /* Maps every x literal to the indices of the clauses where it appears. */
  Map<BLit, Vector<size_t>> appearancesOfLit;

  for (size_t i = 0; i < _defined.size(); i++)
  {
    for (BLit x : _negDefinitions[i])
    {
      appearancesOfLit[x].push_back(i);
      appearancesOfLit[-x]; /* guarantees that there is an entry for the inverse literal as well */
    }
  }

  /* Initialize a graph with one vertex for each clause */
  Vector<size_t> range(_defined.size());
  iota(range.begin(), range.end(), 0);
  Graph<size_t> graph(range);

  /* Adds an edge between every two clauses that have opposite literals */
  for (const auto& entry : appearancesOfLit)
  {
    BLit lit = entry.first;

    const Vector<size_t>& appearances = entry.second;
    const Vector<size_t>& antiAppearances = appearancesOfLit[-lit];

    for (size_t i : appearances)
      for (size_t j : antiAppearances)
      	graph.addEdge(i, j);
  }

  return graph;
}
