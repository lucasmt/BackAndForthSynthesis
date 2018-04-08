#pragma once

#include "Vector.hpp"
#include "Set.hpp"
#include "CNFFormula.hpp"

/**
 * Class representing the function being synthesized.
 *
 * Stores a partition of the z variables into components controlling disjoint y variables,
 * and for each component a list of MSS associated with that component.
 */
class Model
{
	/** List of components as sets of z variables */
	Vector<Set<BVar>> _componentList;

	/** List of MSS for each component */
	Vector<Vector<Set<BVar>>> _componentMSS;

public:

	size_t componentCount() const;
	size_t mssCount() const;

	/** Returns list of components */
	const Vector<Set<BVar>>& allComponents() const;

	/** Returns list of MSS for a given component */
	const Vector<Set<BVar>>& mssForComponent(size_t componentId) const;

	/** Adds component to list of components and returns index as identifier */
	size_t addComponent(Set<BVar> component);

	/** Adds MSS to MSS list of the component with the given identifier */
	void addMSS(size_t componentId, Set<BVar> mss);

	/* Returns true if the given set is a subset of any MSS of the given component */
	bool alreadyCovered(size_t componentId, const Set<BVar>& s);
};
