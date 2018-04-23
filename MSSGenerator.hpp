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
  //openwbo::WBO maxSatSolver; /**< MaxSAT solver used for generating the MSS */

  Set<BVar> allIndicatorVars;

  /** Add hard clause */
  void enforceClause(const CNFClause& clause);

  /** Add hard clause blocking given MSS */
  void blockMSS(const Set<BVar>& mss);

public:

  MSSGenerator(Set<BVar> indicatorVarSet,
	       const Vector<BVar>& indicators,
	       const Vector<CNFClause>& clauses);

  /**
   * Generate new MSS, or nothing if there are no MSS left.
   * MSS is represented by the set of Z and Y variables set to true.
   */
  Optional<Set<BVar>> newMSS();

  /** Generate new MSS containing the given variables, or nothing if there are no MSS left */
  Optional<Set<BVar>> newMSSCovering(const Set<BVar>& vars);
};
