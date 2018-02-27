#include "MSSSpec.hpp"

using std::vector;

MSSSpec::MSSSpec(vector<int> indicatorVars, CNFFormula cnf)
{
  _indicatorVars = indicatorVars; 
  _cnf = cnf;
}

const CNFFormula& MSSSpec::outputCNF() const
{
  return _cnf;
}

const vector<int>& MSSSpec::indicatorVars() const
{
  return _indicatorVars;
}

VarSet MSSSpec::inputVars() const
{
  return VarSet(_indicatorVars.begin(), _indicatorVars.end());
}

VarSet MSSSpec::outputVars() const
{
  return _cnf.vars();
}
