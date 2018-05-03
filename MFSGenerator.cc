#include "MFSGenerator.hpp"
#include "PriorityQueue.hpp"

#include <utility>

using Glucose::vec;
using Glucose::Lit;
using Glucose::mkLit;
using Glucose::lbool;
using std::tuple;
using std::get;
using std::move;

MFSGenerator::MFSGenerator(Set<BVar> relevantIndicators,
                           Vector<BVar> indicatorVars,
                           Graph<size_t> conflictGraph)
	: _relevantIndicators(move(relevantIndicators))
	, _indicatorVars(move(indicatorVars))
	, _conflictGraph(move(conflictGraph))
{
	for (BVar var : _relevantIndicators)
	{
		_appearances[var] = 0;
	}

	for (size_t i = 0; i < _indicatorVars.size(); i++)
	{
		_index[_indicatorVars[i]] = i;		
	}

	_satSolver.setIncrementalMode();

	for (size_t i = 0; i < _indicatorVars.size(); i++)
	{
		_satSolver.newVar();
	}

	/* Add clauses saying that if two vertices are connected they cannot be in the same MFS */
	for (tuple<size_t, size_t> edge : _conflictGraph.edges())
	{
		_satSolver.addClause(~mkLit(get<0>(edge)), ~mkLit(get<1>(edge)));
	}
}

Optional<Set<BVar>> MFSGenerator::newMFS()
{
	/* Solver returned SAT, a falsifiable set was found */
	if (_satSolver.solve())
	{	
		/* Split variables between those in the model and those not in the model */
		Set<BVar> inModel;

		for (int i = 0; i < _satSolver.model.size(); i++)
		{
			if (_satSolver.model[i] == l_True)
				inModel.insert(_indicatorVars[i]);
		}

		/* Set of indicator variables in the model, will be extended to an MFS */
		Set<BVar> mfs = setIntersection(inModel, _relevantIndicators);

		// Initialize a priority queue where the top element is the one in the least MSS seen so far.
		MinQueue<size_t, BVar> pqueue =
			makeMinQueue<size_t, BVar, Map<BVar, size_t>::iterator>(_appearances.begin(), _appearances.end());

		// Try to extend the MFS in order, with priority to elements that have not been in many MSS.
		while (!pqueue.empty())
		{
			BVar v = popTop(pqueue);

			// Test if the vertex is already in the MFS
			if (mfs.find(v) == mfs.end())
			{
				auto isNeighbor = [this, v] (BVar u)
					{
						return _conflictGraph.edgeExists(_index[v], _index[u]);
					};

				/* Test if there is an edge between the new vertex and any vertex already in the MFS */
				bool conflict = std::any_of(mfs.begin(), mfs.end(), isNeighbor);

				/* If there is no edge, then MFS can be extended with new vertex */
				if (!conflict)
					mfs.insert(v);
			}
		}

		return mfs;
	}
	/* Solver returned UNSAT, no remaining MFS */
	else
	{
		return nullopt;
	}
}

void MFSGenerator::blockMSS(const Set<BVar>& mss)
{
	for (BVar inMSS : setIntersection(_relevantIndicators, mss))
	{
		_appearances[inMSS]++;
	}
	
	vec<Lit> glucoseClause;

	for (BVar notInMSS : setDifference(_relevantIndicators, mss))
	{
		glucoseClause.push(mkLit(_index.at(notInMSS)));
	}

	_satSolver.addClause(glucoseClause);
}
