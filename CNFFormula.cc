#include "CNFFormula.hpp"

#include <algorithm>
#include <unordered_map>

using std::vector;
using std::set;
using std::unordered_map;

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

CNFFormula CNFFormula::projection(const set<int>& vars) const
{
  CNFFormula newCNF;

  for (const CNFClause& clause : _clauses)
    {
      CNFClause newClause;

      for (int lit : clause.lits())
	if (vars.find(abs(lit)) != vars.end())
	  newClause |= lit;

      newCNF &= newClause;
    }

  return newCNF;
}

Graph CNFFormula::dependencyGraph() const
{
  Graph g(_clauses.size());

  unordered_map<int,set<int>> appearances;

  for (size_t i = 0; i < _clauses.size(); i++)
    for (int l : _clauses[i].lits())
      appearances[abs(l)].insert(i);

  for (const auto& entry : appearances)
    for (int i : entry.second)
      for (int j : entry.second)
	{
	  g.addEdge(i, j);
	  g.addEdge(j, i);
	}

  return g;
}
