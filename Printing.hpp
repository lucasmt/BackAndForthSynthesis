#pragma once

#include <iostream>

#include "CNFFormula.hpp"
#include "MSSSpec.hpp"
#include "TrivialSpec.hpp"

using std::cout;
using std::endl;

/**
 * Prints a list of literals with the given separator between elements.
 * Example: If lits = [-1, 2, 3, -4] and separator = ", ", prints "~x:1, x:2, x:3, ~x:4".
 */
void print(const Vector<BLit>& lits, const string& separator)
{
  bool first = true;

  for (BLit lit : lits)
  {
    if (first)
      first = false;
    else
      cout << separator;

    cout << (lit < 0 ? "~x:" : "x:") << abs(lit);
  }
}

/**
 * Prints a set of variables in the form "{ x:i_1, x:i_2, ..., x:i_k }",
 * where i_1, ..., i_k are the variable identifiers.
 */
void print(const Set<BVar>& vars)
{
  /* Uses the fact that both BLit and BVar are defined as int */
  Vector<BLit> lits(vars.begin(), vars.end());

  cout << "{ ";
  print(lits, ", ");
  cout << " }";
}

/**
 * Prints the given clause.
 */
void print(const CNFClause& clause)
{
  Vector<BLit> lits(clause.begin(), clause.end());

  cout << "(";
  print(lits, " | ");
  cout << ")";
}

/**
 * Prints the given CNF formula.
 */
void print(const CNFFormula& cnf)
{
  for (const CNFClause& clause : cnf)
  {
    print(clause);
    cout << endl;
  }
}

/**
 * Prints the given specification of the form (z_1 <-> ~(l_1 | ... | l_k)) & ...
 */
void print(const TrivialSpec& spec)
{
  /* Repeats for every definition of the form (z <-> ~(l_1 | ... | l_k)) */
  spec.forEach([] (BVar var, const CNFClause& negDefinition) {
      cout << "x:" << var << " <-> ~";
      print(negDefinition);
      cout << endl;
    });
}

/**
 * Prints the given specification of the form (z_1 -> l_1 | ... | l_k) & ...
 */
void print(const MSSSpec& spec)
{
  /* Repeats for every clause of the form (z -> l_1 | ... | l_k) */
  spec.forEach([] (BVar indicatorVar, const CNFClause& clause) {
      cout << "x:" << indicatorVar << " -> ";
      print(clause);
      cout << endl;
    });
}
    
      
