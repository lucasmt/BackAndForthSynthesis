#include "CNFFormula.hpp"

#include <algorithm>

using std::vector;

/** CNFClause **/

CNFClause::CNFClause() {}

CNFClause::CNFClause(int lit)
{
  _lits = { lit };
}

CNFClause::CNFClause(int l1, int l2)
{
  _lits = { l1, l2 };
}

CNFClause::CNFClause(vector<int> lits)
{
  _lits = lits;
}

CNFClause& CNFClause::operator|=(int lit)
{
  _lits.push_back(lit);

  return *this;
}

const vector<int>& CNFClause::lits() const
{
  return _lits;
}

/** CNFFormula **/

CNFFormula::CNFFormula() {}

CNFFormula::CNFFormula(vector<CNFClause> clauses)
{
  _clauses = clauses;
}

const vector<CNFClause>& CNFFormula::clauses() const
{
  return _clauses;
}

CNFFormula& CNFFormula::operator&=(CNFClause clause)
{
  _clauses.push_back(clause);

  return *this;
}

CNFFormula& CNFFormula::operator&=(CNFFormula other)
{
  _clauses.insert(_clauses.end(),
		  other._clauses.begin(),
		  other._clauses.end());

  return *this;
}

CNFFormula CNFFormula::operator&(const CNFFormula& other) const
{
  CNFFormula cnf(*this);

  cnf &= other;

  return cnf;
}

VarSet CNFFormula::vars() const
{
  VarSet vars;

  for (CNFClause c : _clauses)
    for (int l : c.lits())
      vars.insert(abs(l));

  return vars;
}
