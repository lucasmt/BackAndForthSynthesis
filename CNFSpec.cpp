#include "CNFSpec.hpp"

CNFSpec::CNFSpec(VarSet inputVars, VarSet outputVars, CNFFormula cnf)
{
  _cnf = cnf;

  _inputVars = inputVars;
  _outputVars = outputVars;
}

const CNFFormula& CNFSpec::cnf() const
{
  return _cnf;
}

const VarSet& CNFSpec::inputVars() const
{
  return _inputVars;
}

const VarSet& CNFSpec::outputVars() const
{
  return _outputVars;
}
