#pragma once

#include "CNFFormula.hpp"
#include "Set.hpp"
#include "Vector.hpp"
#include "Optional.hpp"
#include "open-wbo/MaxSATFormula.h"
#include "open-wbo/algorithms/Alg_WBO.h"

/**
 * Class that generates Maximal Satisfiable Subsets using a MaxSAT solver.
 */
class MSSGenerator
{
  openwbo::MaxSATFormula maxSatFormula; /**< formula encoding the constraints for the problem */
  openwbo::WBO maxSatSolver; /**< MaxSAT solver used for generating the MSS */

public:

  MSSGenerator(const Vector<BVar>& indicators,
	       const Vector<CNFClause>& clauses);

  void enforceClause(const CNFClause& clause);

  Optional<Set<BVar>> generateMSS();

  Optional<Set<BVar>> generateMSSCovering(const Set<BVar>& vars);
};
