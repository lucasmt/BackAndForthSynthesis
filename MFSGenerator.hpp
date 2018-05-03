#pragma once

#include "CNFFormula.hpp"
#include "Set.hpp"
#include "Map.hpp"
#include "Vector.hpp"
#include "Graph.hpp"
#include "Optional.hpp"
#include "open-wbo/solvers/glucose4.1/core/Solver.h"

/**
 * Class that generates Maximal Falsifiable Subsets using a SAT solver.
 */
class MFSGenerator
{
	Set<BVar> _relevantIndicators; /*< indicator variables in the current connected component */
	Vector<BVar> _indicatorVars; /*< indicator variables across all components by index */
	Graph<size_t> _conflictGraph; /*< graph where every MIS represents an MFS */
	Map<BVar, size_t> _index; /*< _index[v] == i iff _indicatorVars[i] = v */

	Map<BVar, size_t> _appearances; /*< number of MSS each relevant indicator appears in */

	Glucose::Solver _satSolver; /*< SAT solver used to generate MFS */

public:

	/** Constructs a generator that produces MFS corresponding to MIS in the given conflict graph */
	MFSGenerator(Set<BVar> relevantIndicators,
	             Vector<BVar> indicatorVars,
	             Graph<size_t> conflictGraph);

	/** Generates a new MFS, or nothing if there are no MFS left */
	Optional<Set<BVar>> newMFS();
	
	/** Block an MSS such that future MFS will not be contained in it */
	void blockMSS(const Set<BVar>& mss);
};
