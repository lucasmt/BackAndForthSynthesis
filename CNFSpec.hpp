#pragma once

#include "CNFFormula.hpp"
#include "VarSet.hpp"

class CNFSpec
{
  VarSet _inputVars;
  VarSet _outputVars;
  CNFFormula _cnf;

  public:

  CNFSpec(VarSet inputVars, VarSet outputVars, CNFFormula cnf);

  const CNFFormula& cnf() const;

  const VarSet& inputVars() const;
  const VarSet& outputVars() const;
};
