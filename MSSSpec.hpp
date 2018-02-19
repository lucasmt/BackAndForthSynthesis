#pragma once

#include <vector>

#include "CNFFormula.hpp"
#include "VarSet.hpp"

class MSSSpec
{
  std::vector<int> _toggleVars;
  CNFFormula _cnf;

public:
  
  MSSSpec(std::vector<int> toggleVars, CNFFormula cnf);

  const CNFFormula& outputCNF() const;
  const std::vector<int>& toggleVars() const;

  VarSet inputVars() const;
  VarSet outputVars() const;
};
