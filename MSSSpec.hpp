#pragma once

#include <vector>

#include "CNFFormula.hpp"
#include "VarSet.hpp"

class MSSSpec
{
  std::vector<int> _indicatorVars;
  CNFFormula _cnf;

public:
  
  MSSSpec(std::vector<int> toggleVars, CNFFormula cnf);

  const CNFFormula& outputCNF() const;
  const std::vector<int>& indicatorVars() const;

  VarSet inputVars() const;
  VarSet outputVars() const;

  void print() const;
};
