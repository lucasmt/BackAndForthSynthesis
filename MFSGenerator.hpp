#pragma once

#include "CNFFormula.hpp"
#include "Set.hpp"
#include "Map.hpp"
#include "Vector.hpp"
#include "Graph.hpp"
#include "Optional.hpp"
#include "open-wbo/solvers/glucose4.1/core/Solver.h"

/**
 * Class that generates Maximal Cliques representing Maximal Falsifiable Subsets.
 */
class MFSGenerator
{
	Set<BVar> _relevantIndicators;
	Vector<BVar> _indicatorVars;
	Map<BVar, size_t> _index;
	Map<size_t, Set<size_t>> _edgeRelation;

	Glucose::Solver _satSolver;

public:

	/** Constructs a generator that calls the callback function for every max-clique of the graph. */
	MFSGenerator(Set<BVar> relevantIndicators,
	             const Vector<BVar>& indicatorVars,
	             const Graph<size_t>& graph);

	Optional<Set<BVar>> newMFS();
	
	void blockMSS(const Set<BVar>& mss);
};
