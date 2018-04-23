#include "MFSGenerator.hpp"

using Glucose::vec;
using Glucose::Lit;
using Glucose::mkLit;
using Glucose::lbool;
using std::tuple;
using std::get;

MFSGenerator::MFSGenerator(Set<BVar> relevantIndicators,
			   const Vector<BVar>& indicatorVars,
			   const Graph<size_t>& conflictGraph)
  : _relevantIndicators(relevantIndicators)
  , _indicatorVars(indicatorVars)
  , _edgeRelation(conflictGraph.edgeRelation())
{
  for (size_t i = 0; i < indicatorVars.size(); i++)
  {
    _index[indicatorVars[i]] = i;
  }

  

  //_satSolver.setIncrementalMode();

  for (size_t i = 0; i < indicatorVars.size(); i++)
  {
    _satSolver.newVar();
  }

  for (tuple<size_t, size_t> edge : conflictGraph.edges())
  {
    _satSolver.addClause(~mkLit(get<0>(edge)), ~mkLit(get<1>(edge)));
  }
}

Optional<Set<BVar>> MFSGenerator::newMFS()
{
	if (_satSolver.solve())
	{
		/* Returns SAT, a falsifiable set was found */

		/* Create a vector with the indices of all variables set to true */
		Vector<size_t> inModel;
		Vector<size_t> notInModel;

		for (int i = 0; i < _satSolver.model.size(); i++)
		{
			if (_satSolver.model[i] == l_True)
				inModel.push_back(i);
			else
				notInModel.push_back(i);
		}

		/* Initialize a set of indices in the MFS */
		Set<size_t> mfsIndices(inModel.begin(), inModel.end());

		/* For every variable not in the model, try to extend with that variable */
		for (size_t i : notInModel)
		{
			auto isNeighbor = [this, i] (size_t j)
			{
				return _edgeRelation[i].find(j) != _edgeRelation[i].end();
			};

			bool conflict = std::any_of(mfsIndices.begin(), mfsIndices.end(), isNeighbor);

			if (!conflict)
				mfsIndices.insert(i);
		}

		Set<BVar> mfs;

		for (size_t i : mfsIndices)
			mfs.insert(_indicatorVars[i]);

		return mfs;
	}
	else
	{
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
