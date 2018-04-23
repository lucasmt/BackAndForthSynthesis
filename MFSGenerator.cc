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
{
  for (size_t i = 0; i < indicatorVars.size(); i++)
  {
    _index[indicatorVars[i]] = i;
  }

  _satSolver.setIncrementalMode();

  for (size_t i = 0; i < indicatorVars.size(); i++)
  {
    _satSolver.newVar();
  }

  for (tuple<size_t, size_t> edge : conflictGraph.edges())
  {
    _satSolver.addClause(~mkLit(get<0>(edge)), ~mkLit(get<1>(edge)));
  }
}

/* Given an assignment as a boolean vector, return set of variables set to true */
Set<BVar> variablesSetToTrue(const vec<lbool>& model, const Vector<BVar>& indicatorVars)
{
  Vector<BVar> vars;

  for (int i = 0; i < model.size(); i++)
    if (model[i] == l_True)
      vars.push_back(indicatorVars[i]);

  return Set<BVar>(vars.begin(), vars.end());
}

Optional<Set<BVar>> MFSGenerator::newMFS()
{
  if (_satSolver.solve())
  {
    return variablesSetToTrue(_satSolver.model, _indicatorVars);
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
