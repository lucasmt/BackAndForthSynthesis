#include "MFSGenerator.hpp"

#include <utility>

using Glucose::vec;
using Glucose::Lit;
using Glucose::mkLit;
using Glucose::lbool;
using std::tuple;
using std::get;
using std::move;

#include <chrono>
#include <iostream>

using std::chrono::system_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::cout;
using std::endl;

MFSGenerator::MFSGenerator(Set<BVar> relevantIndicators,
                           Vector<BVar> indicatorVars,
                           Graph<size_t> conflictGraph)
	: _relevantIndicators(relevantIndicators)
	, _indicatorVars(move(indicatorVars))
	, _conflictGraph(move(conflictGraph))
{
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
  auto start = system_clock::now();

	/* Solver returned SAT, a falsifiable set was found */
	if (_satSolver.solve())
	{	
	  auto time = duration_cast<milliseconds>(system_clock::now() - start);

	  //cout << time.count() << " ";

		/* Split variables between those in the model and those not in the model */
		Set<BVar> inModel;
		Set<BVar> notInModel;

		for (int i = 0; i < _satSolver.model.size(); i++)
		{
			if (_satSolver.model[i] == l_True)
				inModel.insert(_indicatorVars[i]);
			else
				notInModel.insert(_indicatorVars[i]);
		}

		//cout << inModel.size() << " ";

		/* Set of indicator variables in the model, will be extended to an MFS */
		Set<BVar> mfs = setIntersection(inModel, _relevantIndicators);

		start = system_clock::now();

		/* For every indicator variable not in the model, try to extend MFS with that variable */
		for (BVar v : setIntersection(notInModel, _relevantIndicators))
		{
			auto isNeighbor = [this, v] (BVar u)
			{
				return _conflictGraph.edgeExists(_index[v], _index[u]);
			};

			// Test if there is an edge between the new vertex and any vertex already in the MFS 
			bool conflict = std::any_of(mfs.begin(), mfs.end(), isNeighbor);

			// If there is no edge, then MFS can be extended with new vertex
			if (!conflict)
				mfs.insert(v);
		}

		time = duration_cast<milliseconds>(system_clock::now() - start);

		//cout << time.count() << " " << mfs.size() << " " << (inModel.size() < mfs.size()) << " ";

		return mfs;
	}
	/* Solver returned UNSAT, no remaining MFS */
	else
	{
	  auto time = duration_cast<milliseconds>(system_clock::now() - start);

	  //cout << time.count() << endl;

		return nullopt;
	}
}

void MFSGenerator::blockMSS(const Set<BVar>& mss)
{
	vec<Lit> glucoseClause;

	for (BVar notInMSS : setDifference(_relevantIndicators, mss))
	{
		glucoseClause.push(mkLit(_index.at(notInMSS)));
	}

	_satSolver.addClause(glucoseClause);
}
