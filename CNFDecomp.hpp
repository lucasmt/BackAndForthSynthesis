#include "CNFChain.hpp"

#include <algorithm>

using std::max;

/**
 * Decomposes a CNF specification into (F_1, F_2) according to the CNF decomposition.
 */
CNFChain cnfDecomp(const CNFSpec& spec)
{
  const Set<BVar>& inputVars = spec.inputVars();
  const Set<BVar>& outputVars = spec.outputVars();

  /* Find variable with maximum id */
  BVar lastVar = max(maxElement(inputVars), maxElement(outputVars));

  Vector<BVar> indicatorVars; /*< z_1, z_2, ..., z_n */
  Vector<CNFClause> inputClauses; /*< X_1, X_2, ..., X_n */
  CNFFormula outputCNF; /*< Y_1 /\ Y_2 /\ ... /\ Y_n */

  for (const CNFClause& clause : spec.cnf())
  {
    /* Add new z_i */
    lastVar++;
    indicatorVars.push_back(lastVar);

    /* Split clause into input and output parts */
    CNFClause inputClause, outputClause;

    for (BLit lit : clause)
    {
      BVar var = abs(lit); /*< remove sign of literal to get the variable */

      if (inputVars.find(var) != inputVars.end()) /*< var is an input variable */
	inputClause |= lit;
      else /*< var is an output variable */
	outputClause |= lit;
    }

    inputClauses.push_back(inputClause);
    outputCNF &= outputClause;
  }

  /* Construct F_1 and F_2 */
  TrivialSpec f1(indicatorVars, move(inputClauses)); /*< make a copy of indicatorVars */
  MSSSpec f2(move(indicatorVars), outputVars, move(outputCNF)); /*< make a copy of outputVars */

  return CNFChain(move(f1), move(f2));
}
