#include "MSSSpec.hpp"

using std::vector;

MSSSpec::MSSSpec(vector<int> toggleVars, CNFFormula cnf)
{
  _toggleVars = toggleVars; 
  _cnf = cnf;
}

const CNFFormula& MSSSpec::outputCNF() const
{
  return _cnf;
}

const vector<int>& MSSSpec::toggleVars() const
{
  return _toggleVars;
}

VarSet MSSSpec::inputVars() const
{
  return VarSet(_toggleVars.begin(), _toggleVars.end());
}

VarSet MSSSpec::outputVars() const
{
  return _cnf.vars();
}
