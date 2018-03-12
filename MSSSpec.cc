#include "MSSSpec.hpp"

#include <utility>

using std::move;
using std::function;

MSSSpec::MSSSpec(Vector<BVar> indicatorVars, CNFFormula outputCNF)
  : _indicatorVars(move(indicatorVars)),
    _outputCNF(move(outputCNF))
{}

const CNFFormula& MSSSpec::outputCNF() const
{
  return _outputCNF;
}

const Vector<BVar>& MSSSpec::indicatorVars() const
{
  return _indicatorVars;
}

void MSSSpec::forEach(function<void(BVar, const CNFClause&)> visitor) const
{
  for (size_t i = 0; i < _indicatorVars.size(); i++)
    visitor(_indicatorVars[i], _outputCNF[i]);
}
