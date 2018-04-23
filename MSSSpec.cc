#include "MSSSpec.hpp"

#include <utility>
#include <algorithm>

using std::move;
using std::function;
using std::max_element;
using std::max;

MSSSpec::MSSSpec(Vector<BVar> indicatorVars,
                 Set<BVar> outputVars,
                 CNFFormula outputCNF)
  : _indicatorVars(move(indicatorVars)),
    _outputVars(move(outputVars)),
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

const Set<BVar>& MSSSpec::outputVars() const
{
	return _outputVars;
}

BVar MSSSpec::maxVar() const
{
	BVar maxIndicator = *max_element(_indicatorVars.begin(), _indicatorVars.end());
	BVar maxOutput = *max_element(_outputVars.begin(), _outputVars.end());

	return max(maxIndicator, maxOutput);
}

void MSSSpec::forEach(function<void(BVar, const CNFClause&)> visitor) const
{
  for (size_t i = 0; i < _indicatorVars.size(); i++)
    visitor(_indicatorVars[i], _outputCNF[i]);
}
