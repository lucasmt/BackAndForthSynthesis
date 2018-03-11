#include "Set.hpp"
#include "Vector.hpp"
#include "CNFFormula.hpp"
#include "MFSGenerator.hpp"
#include "MSSGenerator.hpp"

#include <list>

using std::list;

/*
Set<size_t> nonEmptyIndices(const CNFFormula& cnf)
{
  Set<size_t> nonEmpty;
  
  const Vector<CNFClause>& clauses = cnf.clauses();

  for (size_t i = 0; i < clauses.size(); i++)
    if (!clauses[i].lits().empty())
      nonEmpty.insert(i);

  return nonEmpty;
}
*/

//This is the main algorithm!!!
// Takes f1,f2, returns a list of sets which represents an assignment to both the z variables and the y variables. Description of the implementation
// f1 is a formula from X to Z in a form of (z_i <-> \neg xi and x2 and x3) - (the x part is the negation of the x clause).
// f2 is a formula from Z to Y in the form of z_i --> Cy where Cy is the Y clause.
// *** This algorithm works fine!!! As far as Luacs (3/6/2018) could tell. The main problem is in the next method that uses initial Cy decomposition and is not working.


Vector<Set<BVar>> backAndForthAlgorithm(const TrivialSpec& f1, const MSSSpec& f2)
{
  /* Graph where every maximal clique corresponds to an MFS of F1 */
  Graph<size_t> cliquesGraph = f1.conflictGraph().complement();

  const Vector<BVar>& indicatorVars = f2.indicatorVars();
  const Vector<CNFClause>& outputClauses = f2.outputClauses();

  /* Set of all indicator variables */
  Set<BVar> allIndicatorVars(indicatorVars.begin(), indicatorVars.end());

  /* Initialize MSS generator */
  MSSGenerator mssGen(indicatorVars, outputClauses);
    
  /* MSS will be stored here */
  Vector<Set<BVar>> mssList;

  /* Function to be called whenever a new maximal clique is found */
  auto callback = [&] (const list<int>& maxClique)
    {
      /* Construct MFS corresponding to maximal clique, as a set of indicator variables */
      Set<BVar> mfs;

      for (int vertexIndex : maxClique)
      {
	size_t indicatorVarIndex = cliquesGraph.vertexByIndex(vertexIndex); /*< get graph vertex corresponding to index in the clique */
	BVar indicatorVar = indicatorVars[indicatorVarIndex]; /*< get variable id corresponding to graph vertex */
	mfs.insert(indicatorVar);
      }

      /* Will be an MSS, or nothing if there are no MSS left to generate */
      Optional<Set<BVar>> mss;

      /* Checks if MFS is a subset of one of the previous MSS */
      auto isSuperset = [&mfs] (const Set<BVar>& mss) { return isSubset(mfs, mss); };
      bool mfsAlreadyCovered = any_of(mssList.begin(), mssList.end(), isSuperset);

      if (mfsAlreadyCovered)
	mss = mssGen.generateMSS(); /*< discard MFS and just generate a new MSS */
      else
	mss = mssGen.generateMSSCovering(mfs); /*< generate a new MSS covering the MFS */

      if (!mss)
	return false; /*< if we've run out of MSS, stop generating maximal cliques */
      else
      {
	mssList.push_back(*mss);
	
	/* Enforce that future MSS should not be subsets of this MSS */
	Set<BVar> notInMSS = setDifference(allIndicatorVars, *mss);
	CNFClause atLeastOneNew = CNFClause::atLeastOne(notInMSS);
	mssGen.enforceClause(atLeastOneNew);

	return true; /*< continue searching for maximal cliques */
      }
    }; /* end of callback */

  /* Initialize maximal-clique generator */
  MFSGenerator mfsGen(cliquesGraph, callback);

  /* Run MFS generator; callback will be called whenever a new maximal clique is found */
  mfsGen.run();

  return mssList;
}

/*
vector<VarSet> algorithm(const TrivialSpec& f1, const MSSSpec& f2)
{
  CNFFormula cnf = f2.outputCNF();
  const vector<CNFClause>& clauses = cnf.clauses();
  const vector<int>& indicatorVars = f2.indicatorVars();

  vector<VarSet> implementation;

  vector<set<int>> components =
    cnf.dependencyGraph().connectedComponents();

  for (const set<int>& component : components)
  {
    cout << "Connected component: { ";

    for (int v : component)
      cout << v << ", ";

    cout << "}" << endl;

    CNFFormula projectedCNF = cnf.projection(component);
    set<int> subset = nonEmptyIndices(projectedCNF);
    Graph conflictGraph = f1.conflictGraph().subgraph(subset).complement();
    vector<CNFClause> projectedClauses;
    vector<int> projectedIndicators;

    for (int i : subset)
    {
      projectedClauses.push_back(clauses[i]);
      projectedIndicators.push_back(indicatorVars[i]);
    }

    MSSGenerator mssGen(projectedClauses, projectedIndicators);
  
    VarSet allIndicators(projectedIndicators.begin(), projectedIndicators.end());

    auto callback = [&projectedIndicators, &mssGen, &implementation, &allIndicators, &conflictGraph]
    (const list<int>& clique)
    {
      VarSet mfs;

      for (int v : clique)
      {
	int i = conflictGraph.vertex(v);
	mfs.insert(projectedIndicators[i]);
      }

      cout << "MFS: ";
      mfs.print();
      
      bool success;

      if (mfs.subsetOfAny(implementation))
	success = mssGen.generateMSS();
      else
	success = mssGen.generateMSSCovering(mfs.vars());

      if (success)
      {
	VarSet mss = mssGen.getMSS();
	implementation.push_back(mss);
	VarSet complement = allIndicators.setDifference(mss);
	mssGen.enforceAtLeastOne(complement.vars());

	cout << "MSS: ";
	mss.print();
	cout << "Enforced: ";
	complement.print();
      }

      cout << (success ? "Success!" : "Failure!") << endl;

      return success;
    };

    MFSGenerator mfsGen(conflictGraph, callback);

    mfsGen.run();
  }

  return implementation;
}
*/

