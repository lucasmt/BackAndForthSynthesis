#pragma once

#include "CNFFormula.hpp"
#include "Set.hpp"
#include "Vector.hpp"

/**
 * Second component of the CNF decomposition:
 * (z_1 -> Y_1) /\ (z_2 -> Y_2) /\ ... /\ (z_n -> Y_n)
 */
class MSSSpec
{
  Vector<BVar> _indicatorVars; /**< z_1, z_2, ..., z_n */
  CNFFormula _outputCNF; /**< Y_1 /\ Y_2 /\ ... /\ Y_n */

public:

  MSSSpec(Vector<BVar> indicatorVars, CNFFormula outputCNF);

  const Vector<BVar>& indicatorVars() const;
  const Vector<CNFClause>& outputClauses() const;
};
