#pragma once

#include "Set.hpp"
#include "Vector.hpp"
#include "CNFFormula.hpp"
#include "MFSGenerator.hpp"
#include "MSSGenerator.hpp"
#include "Printing.hpp"
#include "Model.hpp"

#include <stdexcept>

/**
 * Computes a new MSS covering a not-yet-covered MFS, and stored the MSS in the model.
 * - componentId: Identifier for the component the MSS will be associated with.
 * - mfsGen: MFS generator.
 * - mssGen: MSS generator.
 * - model: Function being synthesized, keeps track of the components and MSS associated with it. New MSS will be stored here.
 */
bool computeAndStoreNextMSS(size_t componentId,
			    MFSGenerator& mfsGen,
			    MSSGenerator& mssGen,
			    Model& model)
{
  /*
   * Generate an MFS (represented as a set of indicator variables) that
   * has not been covered yet, or nothing if all MFS have been covered.
   */
  Optional<Set<BVar>> mfs = mfsGen.newMFS();
   
  if (!mfs)
  {
    return false; /*< no MFS left, indicate that enumeration has finished */
  }
  else
  {
#if MYDEBUG >=2  
    printf("Printing MFS:");
    print(mfs, "x");
    printf("\n");
#endif      

    /* Generate an new MSS covering the MFS */
    Optional<Set<BVar>> mss = mssGen.newMSSCovering(*mfs);
      
    if (!mss)
    {
      /* This branch will never be reached if the specification is realizable */
      throw std::invalid_argument("Specification is unrealizable!");
    }
    else
    {
#if MYDEBUG >=2        
      printf("Printing MSS:");
      print(*mss, "z");
      printf("\n");
#endif        
        
      model.addMSS(componentId, *mss);
      mfsGen.blockMSS(*mss);

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
Model BAFAlgorithm(const TrivialSpec& f1, const MSSSpec& f2)                               
{
	/* Graph where every MIS corresponds to an MFS of F1 */
	Graph<size_t> conflictGraph = f1.conflictGraph();

	const Vector<BVar>& indicatorVars = f2.indicatorVars();
	const Vector<CNFClause>& outputClauses = f2.outputCNF().clauses();

	/* Set of all indicator variables */
	Set<BVar> allIndicatorVars(indicatorVars.begin(), indicatorVars.end());

	/* Initialize maximal-clique generator with graph and callback */
	MFSGenerator mfsGen(allIndicatorVars, indicatorVars, conflictGraph);

	/* Initialize MSS generator */
	MSSGenerator mssGen(allIndicatorVars, indicatorVars, outputClauses);
    
	/* Representation of the synthesized function */
	Model model;

	/* Since we are not decomposing specification into connected components,
	 * there is a single component composed of all indicator variables.*/
	size_t componentId = model.addComponent(allIndicatorVars);

	/* Repeat while there are still MSS to be computed */
	while (computeAndStoreNextMSS(componentId, mfsGen, mssGen, model)) {}

#if MYDEBUG >=2     //printing the remaining of the mss
	printf("No more mfs to cover, printing the remaining mss:\n");
	Optional<Set<BVar>> mss;
	mss = mssGen.newMSS();
	while (mss) 
	{
		printf("Printing MSS:");
		print(*mss, "z");
		printf("\n");
		mss = mssGen.newMSS();
	}
#endif
  
	printf("No more mss\n");

	return model;
}

/**
 * Version of BAFAlgorithm that first decomposes specification into connected components. This method over-runs BAFAlgorithm, 
 */
Model BAFConnectedComponents(const TrivialSpec& f1, const MSSSpec& f2)
{
	/* Graph where every MIS corresponds to an MFS of F1 */
	Graph<size_t> conflictGraph = f1.conflictGraph();

	const Vector<BVar>& indicatorVars = f2.indicatorVars();
	const CNFFormula& outputCNF = f2.outputCNF();

	/* Representation of the synthesized function */
	Model model;

	Vector<Set<size_t>> connectedComponents = outputCNF.dualGraph().connectedComponents();

	for (const Set<size_t>& indices : connectedComponents)
	{
#if MYDEBUG
		// printf("Printing Connected Component:\n");   
		//print(indices);
#endif
      
		/* Restrict indicator variables, output clauses and cliques graph to the indices in the connected component */
		Vector<BVar> subIndicatorVars = subsequence(indicatorVars, indices);
		Vector<CNFClause> subOutputClauses = subsequence(outputCNF.clauses(), indices);
		Graph<size_t> conflictSubgraph = conflictGraph.subgraph(indices);
    
#if MYDEBUG >=2  
		printf("**************************************************************************************\n");
		printf("Printing graph components:\n");
		print(subIndicatorVars, "z");
		printf("\n");
		print(subOutputClauses, "y");
#endif

		/* Set of all indicator variables in the component */
		Set<BVar> relevantIndicators(subIndicatorVars.begin(), subIndicatorVars.end());

		/* Add connected component to model and get an identifier for it.
		 * Identifier will be used to associate an MSS with this component. */
		size_t componentId = model.addComponent(relevantIndicators);

		/* Initialize maximal-clique generator with graph and callback */
		MFSGenerator mfsGen(relevantIndicators, indicatorVars, conflictSubgraph);

		/* Initialize MSS generator */
		MSSGenerator mssGen(relevantIndicators, subIndicatorVars, subOutputClauses);

		/* Repeat while there are still MSS to be computed */
		while (computeAndStoreNextMSS(componentId, mfsGen, mssGen, model)) {}
	  
#if MYDEBUG >=2    //printing the remaining of the mss
		printf("No more mfs to cover, printing the remaining mss:\n");
		Optional<Set<BVar>> mss;
		mss = mssGen.newMSS();
		while (mss) 
		{
			printf("Printing MSS:");
			print(*mss, "z");
			printf("\n");
			mss = mssGen.newMSS();
		}
#endif
	}

	return model;
}
