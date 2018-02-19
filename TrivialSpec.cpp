#include "TrivialSpec.hpp"

#include <unordered_set>
#include <unordered_map>

using std::map;
using std::vector;
using std::pair;
using std::unordered_set;
using std::unordered_map;

TrivialSpec::TrivialSpec(const map<int,vector<int>>& def)
{
  for (const pair<int,vector<int>>& entry : def)
  {
    _outputVars.push_back(entry.first);
    _def.push_back(entry.second);

    for (int l : entry.second)
      _inputVars.insert(abs(l));
  }
}

const vector<vector<int>>& TrivialSpec::def() const
{
  return _def;
}

VarSet TrivialSpec::inputVars() const
{
  return _inputVars;
}

VarSet TrivialSpec::outputVars() const
{
  return VarSet(_outputVars.begin(), _outputVars.end());
}

Graph TrivialSpec::conflictGraph() const
{
  unordered_map<int,unordered_set<int>> zImplying;

  for (int i = 0; i < _def.size(); i++)
  {
    for (int x : _def[i])
      zImplying[x].insert(i);
  }

  Graph graph(_def.size());

  for (const auto& entry : zImplying)
  {
    int x = entry.first;

    unordered_set<int> implyingX = entry.second;
    unordered_set<int> implyingNotX = zImplying[-x];

    for (int z1 : implyingX)
      for (int z2 : implyingNotX)
	graph.addEdge(z1, z2);
  }

  return graph;
}
