#pragma once

#include "CNFFormula.hpp"
#include "Set.hpp"
#include "Vector.hpp"

/**
 * Second component of the CNF decomposition:
 * (z_1 -> Y_1) /\ (z_2 -> Y_2) /\ ... /\ (z_n -> Y_n)
 */
struct MSSSpec
{
  Ref<Vector<Var>> _indicatorVars; /**< z_1, z_2, ..., z_n */
  Ref<CNFFormula> _cnf; /**< Y_1 /\ Y_2 /\ ... /\ Y_n */

public:

  MSSSpec(Ref<Vector<Var>> indicatorVars, Ref<CNFFormula> cnf);

  Var indicatorVar(size_t i) const; /**< return z_i */
  CNFClause outputClause(size_t i) const; /**< return Y_i */
};
