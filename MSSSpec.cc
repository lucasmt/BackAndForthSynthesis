#include "MSSSpec.hpp"

#include <iostream>

using std::vector;
using std::cout;
using std::endl;

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

void MSSSpec::print() const
{
  const vector<CNFClause>& clauses = _cnf.clauses();

  for (size_t i = 0; i < _indicatorVars.size(); i++)
  {
    cout << _indicatorVars[i] << " -> ";

    for (int lit : clauses[i].lits())
      cout << lit << " ";

    cout << endl;
  }
}
