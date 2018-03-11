#include "CNFSpec.hpp"

#include <utility>

using std::move;

CNFSpec::CNFSpec(Set<BVar> inputVars, Set<BVar> outputVars, CNFFormula cnf)
  : _inputVars(move(inputVars)),
    _outputVars(move(outputVars)),
    _cnf(move(cnf))
{}

const CNFFormula& CNFSpec::cnf() const
{
  return _cnf;
}

const Set<BVar>& CNFSpec::inputVars() const
{
  return _inputVars;
}

const Set<BVar>& CNFSpec::outputVars() const
{
  return _outputVars;
}
