#pragma once

#include "VarSet.hpp"
#include "Graph.hpp"

class CNFClause
{
  std::vector<int> _lits;

public:

  CNFClause();
  CNFClause(int l);
  CNFClause(int l1, int l2);
  CNFClause(std::vector<int> lits);

  CNFClause& operator|=(int lit);

  const std::vector<int>& lits() const;
};

class CNFFormula
{
  std::vector<CNFClause> _clauses;

public:

  CNFFormula();
  CNFFormula(std::vector<CNFClause> clauses);

  const std::vector<CNFClause>& clauses() const;
  //std::vector<CNFClause>& clauses();

  CNFFormula& operator&=(CNFClause clause);
  CNFFormula& operator&=(CNFFormula other);
  CNFFormula operator&(const CNFFormula& other) const;

  VarSet vars() const;

  CNFFormula projection(const std::set<int>& vars) const;

  Graph dependencyGraph() const;
};

  
