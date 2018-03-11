#include "CNFChain.hpp"

/**
 * Decomposes a CNF specification into (F_1, F_2) according to the CNF decomposition.
 */
CNFChain cnfDecomp(const CNFSpec& spec)
{
  /* Find variable with maximum id */
  Var lastVar = max(inputVars.max(), outputVars.max());

  Vector<Var> indicatorVars; /*< z_1, z_2, ..., z_n */
  Vector<CNFClause> inputClauses; /*< X_1, X_2, ..., X_n */
  CNFFormula outputCNF; /*< Y_1 /\ Y_2 /\ ... /\ Y_n */

  for (const CNFClause& clause : spec.cnf())
  {
    /* Add new z_i */
    indicatorVariables.push_back(lastVar + 1);
    lastVar++;

    /* Split clause into input and output parts */
    CNFClause inputClause, outputClause;

    for (Lit lit : clause)
    {
      Var var = abs(lit); /*< remove sign of literal to get the variable */

      if (spec.isInput(var))
	inputClause |= lit;
      else
	outputClause |= lit;
    }

    inputClauses.push_back(inputClause);
    outputCNF &= outputClause;
  }

  /* Construct F_1 and F_2 */
  TrivialSpec f1(indicatorVars, move(inputClauses)); /*< make a copy of indicatorVars */
  MSSSpec f2(move(indicatorVars), move(outputCNF));

  return CNFChain(move(f1), move(f2));
}
