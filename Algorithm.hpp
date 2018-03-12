#pragma once

#include "Set.hpp"
#include "Vector.hpp"
#include "CNFFormula.hpp"
#include "MFSGenerator.hpp"
#include "MSSGenerator.hpp"

#include <list>
#include <functional>

/**
 * Constructs the callback function to be called whenever a new maximal clique is found.
 * - cliquesGraph: Graph from which the cliques were extracted, used to translate indices back to vertices.
 * - indicatorVars: Array of all indicator variables, used to get the variable corresponding to a graph vertex.
 * - mssGen: MSS generator.
 * - allIndicatorVars: Set with all indicator variables, used to complement and block an MSS.
 * - mssList: List of computed MSS, where the next MSS will be stored.
 */
std::function<bool(const std::list<int>&)> computeAndStoreNextMSS(const Graph<size_t>& cliquesGraph,
								  const Vector<BVar>& indicatorVars,
								  MSSGenerator& mssGen,
								  const Set<BVar>& allIndicatorVars,
								  Vector<Set<BVar>>& mssList)
{
  return [&] (const std::list<int>& maxClique)
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
    };
}


 /**
  * Back-and-forth synthesis algorithm. Assumes specification is realizable.
  * Input: F1 (specification from X to Z of the form e.g. (z_1 <-> ~(x_1 | ~x_2 | x_3)) & ...
  *        F2 (specification from Z to Y of the form e.g. (z_1 -> (~y_1 | ~y_2 | y_3)) & ...
  * Output: List of sets representing assignments to the Z and Y variables, such that
  *         given the assignment to the Zs, the assignment to the Ys satisfies F2.
  */
Vector<Set<BVar>> BAFAlgorithm(const TrivialSpec& f1, const MSSSpec& f2)
{
  /* Graph where every maximal clique corresponds to an MFS of F1 */
  Graph<size_t> cliquesGraph = f1.conflictGraph().complement();

  const Vector<BVar>& indicatorVars = f2.indicatorVars();
  const Vector<CNFClause>& outputClauses = f2.outputCNF().clauses();

  /* Set of all indicator variables */
  Set<BVar> allIndicatorVars(indicatorVars.begin(), indicatorVars.end());

  /* Initialize MSS generator */
  MSSGenerator mssGen(indicatorVars, outputClauses);
    
  /* MSS will be stored here */
  Vector<Set<BVar>> mssList;

  /* Initialize maximal-clique generator with graph and callback */
  MFSGenerator mfsGen(cliquesGraph,
		      computeAndStoreNextMSS(cliquesGraph,
					     indicatorVars,
					     mssGen,
					     allIndicatorVars,
					     mssList));

  /* Run MFS generator; callback will be called whenever a new maximal clique is found */
  mfsGen.run();

  return mssList;
}

/**
 * Version of BAFAlgorithm that first decomposes specification into connected components.
 */
Vector<Set<BVar>> BAFConnectedComponents(const TrivialSpec& f1, const MSSSpec& f2)
{
  /* Graph where every maximal clique corresponds to an MFS of F1 */
  Graph<size_t> cliquesGraph = f1.conflictGraph().complement();

  const Vector<BVar>& indicatorVars = f2.indicatorVars();
  const CNFFormula& outputCNF = f2.outputCNF();

  /* MSS will be stored here */
  Vector<Set<BVar>> mssList;

  Vector<Set<size_t>> connectedComponents = outputCNF.dualGraph().connectedComponents();

  for (const Set<size_t>& indices : connectedComponents)
  {
    /* Restrict indicator variables, output clauses and cliques graph to the indices in the connected component */
    Vector<BVar> subIndicatorVars = subsequence(indicatorVars, indices);
    Vector<CNFClause> subOutputClauses = subsequence(outputCNF.clauses(), indices);
    Graph<size_t> cliquesSubgraph = cliquesGraph.subgraph(indices);

    /* Set of all indicator variables */
    Set<BVar> allIndicatorVars(subIndicatorVars.begin(), subIndicatorVars.end());

    /* Initialize MSS generator */
    MSSGenerator mssGen(subIndicatorVars, subOutputClauses);
    
    /* Initialize maximal-clique generator with graph and callback */
    MFSGenerator mfsGen(cliquesSubgraph,
			computeAndStoreNextMSS(cliquesSubgraph,
					       indicatorVars, /*< use the original indicatorVars array to get the variable from the index */
					       mssGen,
					       allIndicatorVars,
					       mssList));

    /* Run MFS generator; callback will be called whenever a new maximal clique is found */
    mfsGen.run();
  }

  return mssList;
}
 
