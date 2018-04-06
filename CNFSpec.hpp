#pragma once

#include "CNFFormula.hpp"
#include "Set.hpp"

/**
 * CNF specification composed of:
 * - CNF formula;
 * - Partition of the set of variables into input and output.
 */
class CNFSpec
{
  Set<BVar> _inputVars;
  Set<BVar> _outputVars;
  CNFFormula _cnf;

public:

    
  CNFSpec(Set<BVar> inputVars, Set<BVar> outputVars, CNFFormula cnf);

  const Set<BVar>& inputVars() const;
  const Set<BVar>& outputVars() const;
  const CNFFormula& cnf() const;
};
