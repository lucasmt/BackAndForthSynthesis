#include "MSSSpec.hpp"

#include <utility>

using std::move;

MSSSpec::MSSSpec(Vector<BVar> indicatorVars, CNFFormula outputCNF)
  : _indicatorVars(move(indicatorVars)),
    _outputCNF(move(outputCNF))
{}

const Vector<CNFClause>& MSSSpec::outputClauses() const
{
  return _outputCNF.clauses();
}

const Vector<BVar>& MSSSpec::indicatorVars() const
{
  return _indicatorVars;
}
