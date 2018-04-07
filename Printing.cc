#include "Printing.hpp"

using std::cout;
using std::endl;
using std::string;
/**
 * Prints a list of literals with the given separator between elements using the given variable name.
 * Example: If lits = [-1, 2, 3, -4], separator = ", " and varName = "x", prints "~x:1, x:2, x:3, ~x:4".
 */
void print(const Vector<BLit>& lits, const string& separator, const string& varName)
{
  bool first = true;

  for (BLit lit : lits)
  {
    if (first)
      first = false;
    else
      cout << separator;

    cout << (lit < 0 ? "~" : "") << varName << ":" << abs(lit);
  }
}

/**
 * Prints a set of variables in the form "{ x:i_1, x:i_2, ..., x:i_k }",
 * where i_1, ..., i_k are the variable identifiers and x is the given variable name.
 */
void print(const Set<BVar>& vars, const string& varName)
{
  /* Uses the fact that both BLit and BVar are defined as int */
  Vector<BLit> lits(vars.begin(), vars.end());

  cout << "{ ";
  print(lits, ", ", varName);
  cout << " }";
}

/**
 * Prints a vector of variables as a set in the form "{ x:i_1, x:i_2, ..., x:i_k }",
 * where i_1, ..., i_k are the variable identifiers and x is the given variable name.
 */
void print(const Vector<BVar>& vars, const string& varName)
{
  Set<BVar> varSet(vars.begin(), vars.end());

  print(varSet, varName);
}

/**
 * Prints the given clause.
 */
void print(const CNFClause& clause, const string& varName)
{
  Vector<BLit> lits(clause.begin(), clause.end());

  cout << "(";
  print(lits, " | ", varName);
  cout << ")";
}

/**
 * Prints the given CNF clauses.
 */
void print(const Vector<CNFClause>& clauses, const string& varName)
{
  for (const CNFClause& clause : clauses)
  {
	  print(clause, varName);
    cout << endl;
  }
}

/**
 * Prints the given CNF formula.
 */
void print(const CNFFormula& cnf, const string& varName)
{
  for (const CNFClause& clause : cnf)
  {
	  print(clause, varName);
    cout << endl;
  }
}

/**
 * Prints the given CNF specification using the names given for the input and outpus variables.
 */
void print(const CNFSpec& spec, const string& inputVarName, const string& outputVarName)
{
	for (const CNFClause& clause : spec.cnf())
	{
		cout << "(";

		bool first = true;

		for (BLit lit : clause)
		{
			if (first)
				first = false;
			else
				cout << " | ";
			
			bool isNegative = lit < 0;
			BVar var = abs(lit);
			bool isInput = spec.inputVars().find(var) != spec.inputVars().end();
			
			cout << (isNegative ? "~" : "") << (isInput ? inputVarName : outputVarName) << ":" << var;
		}

		cout << ")" << endl;
	}
}

/**
 * Prints the given specification of the form (z_1 <-> ~(l_1 | ... | l_k)) & ...,
 * where z_1 is an output variable and l_1, ..., l_k are input-variable literals.
 */
void print(const TrivialSpec& spec, const string& inputVarName, const string& outputVarName)
{
  /* Repeats for every definition of the form (z <-> ~(l_1 | ... | l_k)) */
	spec.forEach([&inputVarName, &outputVarName] (BVar var, const CNFClause& negDefinition) {
	  cout << outputVarName << ":" << var << " <-> ~";
	  print(negDefinition, inputVarName);
      cout << endl;
    });
}

/**
 * Prints the given specification of the form (z_1 -> l_1 | ... | l_k) & ...,
 * where z_1 is an input variable and l_1, ..., l_k are output-variable literals.
 */
void print(const MSSSpec& spec, const string& inputVarName, const string& outputVarName)
{
  /* Repeats for every clause of the form (z -> l_1 | ... | l_k) */
	spec.forEach([&inputVarName, &outputVarName] (BVar indicatorVar, const CNFClause& clause) {
	  cout << inputVarName << ":" << indicatorVar << " -> ";
	  print(clause, outputVarName);
      cout << endl;
    });
}
