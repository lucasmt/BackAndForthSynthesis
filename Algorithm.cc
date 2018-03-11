
using std::list;
using std::chrono::system_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

set<int> nonEmptyIndices(const CNFFormula& cnf)
{
  set<int> nonEmpty;
  
  const vector<CNFClause>& clauses = cnf.clauses();

  for (size_t i = 0; i < clauses.size(); i++)
    if (!clauses[i].lits().empty())
      nonEmpty.insert(i);

  return nonEmpty;
}

void printMFS(const VarSet& mfs)
{
  cout << "MFS: ";

  mfs.print();
}

void printMSS(const VarSet& mss, const VarSet& outputSet)
{
  cout << "MSS: ";

  VarSet indicatorSet = mss.setDifference(outputSet);

  indicatorSet.print();
  
  cout << "Satisfying assignment: {";
  
  const set<int>& mssVars = mss.vars();
  const set<int>& outputVars = outputSet.vars();

  for (int var : outputVars)
  {
    cout << "y" << var << ": " << (mssVars.find(var) != mssVars.end()) << ", ";
  }

  cout << "}" << endl;
}

//This is the main algorithm!!!
// Takes f1,f2, returns a list of sets which represents an assignment to both the z variables and the y variables. Description of the implementation
// f1 is a formula from X to Z in a form of (z_i <-> \neg xi and x2 and x3) - (the x part is the negation of the x clause).
// f2 is a formula from Z to Y in the form of z_i --> Cy where Cy is the Y clause.
// *** This algorithm works fine!!! As far as Luacs (3/6/2018) could tell. The main problem is in the next method that uses initial Cy decomposition and is not working.


Vector<Set<Var>> backAndForthAlgorithm(const TrivialSpec& f1, const MSSSpec& f2)
{
  /* Graph where every maximal clique corresponds to an MFS of F1 */
  Graph<size_t> cliquesGraph = f1.conflictGraph().complement();

  const Vector<Var>& indicatorVars = f2.indicatorVars();
  const Vector<CNFClause>& outputClauses = f2.outputClauses();

  /* Set of all indicator variables */
  Set<Var> allIndicatorVars(indicatorVars);

  /* Initialize MSS generator */
  MSSGenerator mssGen(indicatorVars, outputClauses);
    
  /* MSS will be stored here */
  Vector<Set<Var>> mssList;

  /* Function to be called whenever a new maximal clique is found */
  auto callback = [&] (const list<int>& maxClique)
    {
      /* Construct MFS corresponding to maximal clique, as a set of indicator variables */
      Set<Var> mfs;

      for (int vertexIndex : maxClique)
      {
	size_t indicatorVarIndex = graph.vertexByIndex(vertexIndex); /*< get graph vertex corresponding to index in the clique */
	Var indicatorVar = indicatorVars[indicatorVarIndex]; /*< get variable id corresponding to graph vertex */
	mfs.insert(indicatorVar);
      }

      /* Will be an MSS, or nothing if there are no MSS left to generate */
      optional<Set<Var>> mss;

      /* Checks if MFS is a subset of one of the previous MSS */
      bool mfsAlreadyCovered = any_of(mssList.begin(), mssList.end(), mfs.subsetOf);

      if (mfsAlreadyCovered)
	potentialMSS = mssGen.generateMSS(); /*< discard MFS and just generate a new MSS */
      else
	potentialMSS = mssGen.generateMSSCovering(mfs); /*< generate a new MSS covering the MFS */

      if (!mss)
	return false; /*< if we've run out of MSS, stop generating maximal cliques */
      else
      {
	mssList.push_back(*mss);
	
	/* Enforce that future MSS should not be subsets of this MSS */
	Set<Var> notInMSS = allIndicatorVars.setDifference(mss);
	CNFClause atLeastOneNew = CNFClause::atLeastOne(notInMSS);
	mssGen.addHardClause(atLeastOneNew);

	return true; /*< continue searching for maximal cliques */
      }
    }; /* end of callback */

  /* Initialize maximal-clique generator */
  MFSGenerator maxCliqueGen(cliquesGraph, callback);

  /* Run maximal-clique generator; callback will be called whenever a new maximal clique is found */
  maxCliqueGen.run();

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
int main(int argc, char** argv) //receives the path to the input file.
{
  if (argc < 2)
  {
    cout << "Expected format: decomp <input-file>" << endl;
  }
  else
  {
    try
    {
       //Reads input 
      string inputPath(argv[1]);  

      //Parses the file into CNF specification
      CNFSpec f = loadDIMACS(inputPath);

      //Takes specification splits to F1,F2 - in a form of formulas
      CNFChain cnfChain = cnfDecomp(f);

      cout << "F1:" << endl;
      cnfChain.first().print();
      cout << endl;

      cout << "F2:" << endl;
      cnfChain.second().print();
      cout << endl;

      auto start = system_clock::now();

      //Calls the algorithm(F1,F2)
      vector<VarSet> implementation =
	algorithm(cnfChain.first(), cnfChain.second());

      auto t = duration_cast<milliseconds>(system_clock::now() - start);

      cout << implementation.size() << " sets" << endl;
      cout << t.count() << "ms" << endl;
      /*
      for (const VarSet& assignment : implementation)
      {
	cout << "{ ";
	
	for (int v : assignment.vars())
	  cout << v << ", ";

	cout << "}" << endl;
      }
      */
    }
    catch (const runtime_error& e)
    {
      cout << e.what() << endl;
    }
  }
}
