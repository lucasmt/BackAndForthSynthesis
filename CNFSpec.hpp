#pragma once

#include "CNFFormula.hpp"
#include "Set.hpp"
#include "Ref.hpp"

/**
 * CNF specification composed of:
 * - CNF formula;
 * - Partition of the set of variables into input and output.
 */
class CNFSpec
{
  Set<Var> _inputVars;
  Set<Var> _outputVars;
  CNFFormula _cnf;

public:

  CNFSpec(Set<Var> inputVars, Set<Var> outputVars, CNFFormula cnf);
};
