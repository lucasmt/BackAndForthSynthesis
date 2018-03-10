#pragma once

#include "CNFFormula.hpp"
#include "Graph.hpp"
#include "Vector.hpp"
#include "Set.hpp"
#include "Ref.hpp"

/**
 * Second component of the CNF decomposition:
 * (z_1 <-> ~X_1) /\ (z_2 <-> ~X_2) /\ ... /\ (z_n <-> ~X_n)
 */
class TrivialSpec
{
  Ref<Vector<Var>> _defined; /**< z_1, z_2, ..., z_n */
  Ref<Vector<CNFClause>> _negDefinitions; /**< X_1, X_2, ..., X_n */
  
public:
  
  TrivialSpec(Ref<Vector<Var>> defined, Ref<Vector<CNFClause>> negDefinition);

  /**
   * Returns graph where:
   * - Vertex i represents clause X_i;
   * - There is an edge between two vertices iff the clauses have opposite literals.
   */
  Graph<size_t> conflictGraph() const;
};
