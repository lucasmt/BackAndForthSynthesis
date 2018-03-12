#pragma once

#include <fstream>
#include <tuple>

#include "CNFFormula.hpp"
#include "Set.hpp"
#include "CNFSpec.hpp"

using std::runtime_error;
using std::string;
using std::ifstream;
using std::tuple;
using std::make_tuple;
using std::get;
using std::move;

/**
 * Skip all comment lines.
 */
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

/**
 * Reads header of DIMACS file in format (p cnf <var-count> <clause-count>).
 */
tuple<int, int> readHeader(ifstream& in)
{
  string p, cnf;
  in >> p >> cnf;

  if (p != "p" && cnf != "cnf")
  {
    throw runtime_error(
	  "Incorrect format of DIMACS file: expected \"p cnf\", got \"" +
	  p + " " + cnf + "\"");
  }

  size_t varCount, clauseCount;
  in >> varCount >> clauseCount;

  return make_tuple(varCount, clauseCount);
}

/**
 * Reads list of quantified variables from the input string.
 */
Set<BVar> readQuantifiedVars(ifstream& in, const string& quantifier)
{
  string q;
  in >> q;

  /* Check that the quantifier is the one we expected */
  if (q != quantifier)
  {
      throw runtime_error("Incorrect format of DIMACS file: expected \"" +
			  quantifier + "\", got \"" + q + "\"");
  }

  Set<BVar> vars;
  
  BVar v;
  in >> v;

  /* Read variables until the 0 delimiter */
  while (!in.eof() && v != 0)
  {
      vars.insert(v);
      in >> v;
  }

  /* Error if file ended before finding a 0 */
  if (in.eof())
  {
      throw runtime_error("Unexpected end of file while reading DIMACS file");
  }

  return vars;
}

/**
 * Reads the given number of clauses from the input stream.
 */
CNFFormula readClauses(ifstream& in, size_t clauseCount)
{
  CNFFormula cnf;

  for (size_t i = 0; i < clauseCount; i++)
  {
    CNFClause clause;

    BLit lit;
    in >> lit;

    /* Read literals until the 0 delimiter */
    while (!in.eof() && lit != 0)
    {
      clause |= lit;

      in >> lit;
    }

    /* Error if file ended before finding a 0 */
    if (lit != 0)
    {
      throw runtime_error("Unexpected end of file while reading DIMACS file");
    }

    cnf &= clause;
  }

  return cnf;
}


/**
 * Reads a forall-exists QDIMACS file and parses it into a specification in CNF.
 * Assumes that all comments are located at the start of the file.
 */
CNFSpec loadDIMACS(const string& path)
{
  ifstream in(path);

  if (!in.is_open())
    throw runtime_error("Unable to open file " + path);

  skipComments(in);

  size_t clauseCount = get<1>(readHeader(in)); /**< get only the number of clauses */

  Set<BVar> inputVars = readQuantifiedVars(in, "a"); /**< read list of universal variables */ 
  Set<BVar> outputVars = readQuantifiedVars(in, "e"); /**< read list of existential variables */

  CNFFormula cnf = readClauses(in, clauseCount);

  return CNFSpec(move(inputVars), move(outputVars), move(cnf));
}
