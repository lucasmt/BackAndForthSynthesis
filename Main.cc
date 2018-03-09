#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <map>
#include <utility>
#include <algorithm>
#include <list>
#include <set>
#include <chrono>

#include "CNFChain.hpp"
#include "CNFFormula.hpp"
#include "CNFSpec.hpp"
#include "MSSSpec.hpp"
#include "TrivialSpec.hpp"
#include "VarSet.hpp"
#include "Graph.hpp"
#include "MSSGenerator.hpp"
#include "MFSGenerator.hpp"

using std::cout;
using std::endl;
using std::runtime_error;
using std::string;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::unordered_map;
using std::map;
using std::pair;
using std::make_pair;
using std::move;
using std::max;
using std::list;
using std::set;
using std::chrono::system_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

void skipComments(ifstream& in)
{
  string line;

  while (in.peek() == 'c')
  {
      getline(in, line);
  }

  if (in.eof())
  {
      throw runtime_error("Unexpected end of file while reading DIMACS file");
  }
}



pair<int, int> readHeader(ifstream& in)
{
  string p, cnf; in >> p >> cnf;

  if (p != "p" && cnf != "cnf")
  {
    throw runtime_error(
	  "Incorrect format of DIMACS file: expected \"p cnf\", got \"" +
	  p + " " + cnf + "\"");
  }

  int nVars, nClauses;
  in >> nVars >> nClauses;

  return make_pair(nVars, nClauses);
}



VarSet readQuantifiedSet(ifstream& in, const string& quantifier)
{
  string q; in >> q;

  if (q != quantifier)
  {
      throw runtime_error("Incorrect format of DIMACS file: expected \"" +
			  quantifier + "\", got \"" + q + "\"");
  }

  VarSet vars;
  int v; in >> v;

  while (!in.eof() && v != 0)
  {
      vars.insert(v);
      in >> v;
  }

  if (in.eof())
  {
      throw runtime_error("Unexpected end of file while reading DIMACS file");
  }

  return vars;
}



CNFFormula readClauses(ifstream& in, int nClauses)
{
  vector<CNFClause> clauses(nClauses);

  for (int i = 0; i < nClauses; i++)
  {
      int lit;

      in >> lit;

      while (lit != 0)
      {
          clauses[i] |= lit;

          in >> lit;
      }
  }

  return CNFFormula(clauses);
}


//Reads the input file and parses into a formula F in a specific format
CNFSpec loadDIMACS(const string& path)
{
  ifstream in(path);

  if (!in.is_open())
    throw runtime_error("Unable to open file " + path);

  skipComments(in);

  pair<int, int> counts = readHeader(in);

  VarSet inputVars = readQuantifiedSet(in, "a");
  VarSet outputVars = readQuantifiedSet(in, "e");

  CNFFormula cnf = readClauses(in, counts.second);

  return CNFSpec(move(inputVars), move(outputVars), move(cnf));
}


//Decomposes the CNF formula into formulas F1,F2
CNFChain cnfDecomp(const CNFSpec& spec)
{
  const CNFFormula& cnf = spec.cnf();
  const VarSet& inputVars = spec.inputVars();
  const VarSet& outputVars = spec.outputVars();

  int z = max(inputVars.max(), outputVars.max());

  map<int,vector<int>> def;
  vector<int> toggleVars;
  CNFFormula outputCNF;

  for (const CNFClause& c : cnf.clauses())
  {
    z++;
    toggleVars.push_back(z);
    CNFClause outputClause;

    def[z];

    for (int l : c.lits())
    {
      if (inputVars.isElem(abs(l)))
        def[z].push_back(-l);
      else
        outputClause |= l;
    }

    outputCNF &= outputClause;
  }

  TrivialSpec f1(move(def));
  MSSSpec f2(move(toggleVars), move(outputCNF));

  return CNFChain(move(f1), move(f2));
}




void writeGraph(const Graph& graph, const string& path)
{
  ofstream out(path);
  int n = graph.size();
  int m = graph.nEdges();
  
  out << n << " " << m << endl;
  
  const vector<vector<int>>& adjacencyMatrix = graph.adjacencyMatrix();
  
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      if (adjacencyMatrix[i][j] == 1)
        out << i << "," << j << endl;
}



void writeCNF(const CNFFormula& cnf, const string& path)
{
  ofstream out(path);

  int nVars = cnf.vars().max();
  int nClauses = cnf.clauses().size();

  out << "p cnf " << nVars << " " << nClauses << endl;

  for (const CNFClause& clause : cnf.clauses())
  {
    for (int lit : clause.lits())
      out << lit << " ";

    out << "0" << endl;
  }
}



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


vector<VarSet> algorithm(const TrivialSpec& f1, const MSSSpec& f2)
{
    
  Graph graph = f1.conflictGraph().complement(); // constructs the graph from f1, from which we extract the MIS. ConflictGraph() gives the MIS, complement() gives the cliques graph.
  
  CNFFormula cnf = f2.outputCNF(); //Take the Y part of the formula f2 (later to be MSS'd)
  const vector<CNFClause>& clauses = cnf.clauses(); 
  
  const vector<int>& indicatorVars = f2.indicatorVars(); //the z variables from f2 in a specific order (index i holds zi).
  
  VarSet allIndicators(indicatorVars.begin(), indicatorVars.end()); //  ?

  vector<VarSet> implementation; // Creating the return object.

  MSSGenerator mssGen(clauses, indicatorVars);//from this generator we will get the MSS.
  
  
  
  auto callback = [&indicatorVars, &mssGen, &implementation, &allIndicators, &graph, &f2]  //in [] is the objects that the callback function uses.
    (const list<int>& clique) //called whenever a new clique is constructed in a form of a list of indices in the graph
    {
        
      VarSet mfs;

      for (int v : clique)   //for every index in the maximal clique (the MFS) we are getting the z variable that corresponds to the specific vertex.
      {
	int i = graph.vertex(v);   //we get for what zi this vertex of the graph corresponds to.
	mfs.insert(indicatorVars[i]);  //getting that zi, and inserting it to form the MFS.  So now we get the MFS with the indices of the origial formula.
      }

      printMFS(mfs);
      
      bool success; //sucess - we managed to generate a new MSS  Otherwise we ran out of MSS.

      if (mfs.subsetOfAny(implementation))   //checks if this MFS that we obtained was covered before.
	success = mssGen.generateMSS();  // If it was, we do not store it, but generate an MSS (just the next in line).
      else
	success = mssGen.generateMSSCovering(mfs.vars());  // else we generate a specific MSS that covers the MFS.

      if (success) //we managed to generate a new MSS
      {
	VarSet mss = mssGen.getMSS(); //we got the MSS from the generator.
	
        implementation.push_back(mss); // We push the MSS to our list.
        
	VarSet complement = allIndicators.setDifference(mss); // We take the complement of the MSS. Since we need to add a new clause to the MSS generator to tell the generator not to generate the same MSS again or a subset of it.
	
        mssGen.enforceAtLeastOne(complement.vars()); //Adding a clause that says that at least one variable that is not in the previous MSS has to be in the new one. (otherwise we already contained in a previous one).

        // We do not deduce anything about the cliques from the MSS because such deduction is a non trivial problem.
        
        
	printMSS(mss, f2.outputVars());
	//	cout << "Enforced: ";
	//	complement.print();
      }

      //      cout << (success ? "Success!" : "Failure!") << endl;

      return success;
      
    }; //end callback function.

    
    
    
  MFSGenerator mfsGen(graph, callback); //We initialize an MFS generator that uses the graphs and the callback.

  mfsGen.run(); //running the generator

  return implementation;
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
