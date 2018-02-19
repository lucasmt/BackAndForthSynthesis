#pragma once

#include <map>
#include <vector>

#include "Graph.hpp"
#include "VarSet.hpp"

class TrivialSpec
{
  VarSet _inputVars;
  std::vector<int> _outputVars;
  std::vector<std::vector<int>> _def;
  
public:
  
  TrivialSpec(const std::map<int,std::vector<int>>& def);

  const std::vector<std::vector<int>>& def() const;

  VarSet inputVars() const;
  VarSet outputVars() const;

  Graph conflictGraph() const;
};
