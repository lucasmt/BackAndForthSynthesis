#pragma once

#include "CNFFormula.hpp"
#include "Set.hpp"
#include "Ref.hpp"

/**
 * CNF specification composed of:
 * - CNF formula;
 * - Partition of the set of variables into input and output.
 */
struct CNFSpec
{
  const Ref<Set<Var>> inputVars;
  const Ref<Set<Var>> outputVars;
  const Ref<CNFFormula> cnf;
};
