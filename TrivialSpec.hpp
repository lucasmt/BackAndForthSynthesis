#pragma once

#include "CNFFormula.hpp"
#include "Graph.hpp"
#include "Vector.hpp"
#include "Set.hpp"

#include <functional>

/**
 * Second component of the CNF decomposition:
 * (z_1 <-> ~X_1) /\ (z_2 <-> ~X_2) /\ ... /\ (z_n <-> ~X_n)
 */
class TrivialSpec
{
	Vector<BVar> _defined; /**< z_1, z_2, ..., z_n */
	Vector<CNFClause> _negDefinitions; /**< X_1, X_2, ..., X_n */
  
public:
  
	TrivialSpec(Vector<BVar> defined, Vector<CNFClause> negDefinitions);

	/**
	 * Iterates over definitions, executing the visitor function for each.
	 * Example: For a definition of the form (z <-> ~(l_1 | ... | l_k)),
	 * the inputs given to the visitor would be (z, (l_1, ..., l_k)).
	 */
	void forEach(std::function<void(BVar, const CNFClause&)> visitor) const;

	/**
	 * Returns graph where:
	 * - Vertex i represents clause X_i;
	 * - There is an edge between two vertices iff the clauses have opposite literals.
	 */
	Graph<size_t> conflictGraph() const;

	/**
	 * Evaluates the function given by the specification on an assignment to the input variables.
	 */
	Set<BVar> eval(const Set<BVar>& assignment) const;
};
