#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <map>
#include <utility>
#include <algorithm>
#include <list>

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

vector<VarSet> algorithm(const TrivialSpec& f1, const MSSSpec& f2)
{
  Graph graph = f1.conflictGraph().complement();
  CNFFormula cnf = f2.outputCNF();
  const vector<CNFClause>& clauses = cnf.clauses();
  const vector<int>& indicatorVars = f2.indicatorVars();

  MSSGenerator mssGen(clauses, indicatorVars);
  
  vector<VarSet> implementation;

  auto callback = [&indicatorVars, &mssGen, &implementation]
    (const list<int>& clique)
    {
      VarSet mfs;

      for (int v : clique)
	mfs.insert(indicatorVars[v]);
      
      bool success;

      if (mfs.subsetOfAny(implementation))
	success = mssGen.generateMSS();
      else
	success = mssGen.generateMSSCovering(mfs.vars());

      if (success)
      {
	VarSet mss = mssGen.getMSS();
	implementation.push_back(mss);
	mssGen.blockSubset(mss.vars());
      }

      return success;
    };

  MFSGenerator mfsGen(graph, callback);

  mfsGen.run();

  return implementation;
}

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    cout << "Expected format: decomp <input-file>" << endl;
  }
  else
  {
    try
    {
      string inputPath(argv[1]);
      //string graphPath(argv[2]);
      //string cnfPath(argv[3]);

      CNFSpec f = loadDIMACS(inputPath);

      CNFChain cnfChain = cnfDecomp(f);

      algorithm(cnfChain.first(), cnfChain.second());
    }
    catch (const runtime_error& e)
    {
      cout << e.what() << endl;
    }
  }
}

