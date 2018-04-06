#pragma once

#include "Vector.hpp"
#include "Set.hpp"
#include "Graph.hpp"

/**
 * Type aliases for representing CNF variables and literals.
 * A BVar should always be positive.
 * A BLit can be either positive or negative.
 */
using BVar = int;
using BLit = int;

//DF: 4.6.2018 A simple method of checking whether a literal is negated or unnegated variable.
bool isPositive(BLit); 

/**
 * Clause in a CNF formula: (l_1 \/ l_2 \/ ... \/ l_k). DF: k>0
 */
class CNFClause
{
  Vector<BLit> _lits; /**< list of literals in the clause */

public:

  CNFClause(); /**< construct a clause with no literals (equivalent to false) */
  CNFClause(BLit l); /**< construct a clause with a single literal */
  CNFClause(BLit l1, BLit l2); /**< construct a clause with two literals */

  CNFClause& operator|=(BLit lit); /**< add a new literal to the clause */
  CNFClause& operator|=(const CNFClause& other); /**< add all literals in other clause to this clause */
  CNFClause operator|(const CNFClause& other) const; /**< create a new clause from the OR of two clauses */

  static CNFClause atLeastOne(const Set<BVar>& vars);

  CNFClause projection(const Set<BVar>& vars) const; /**< remove from clause all variables not in the set */
  
  bool eval(const Set<BVar>& asgn) const; // DF 4/6/2018  given an assignment asgn, we return True iff the assignment evals true for that (disjunctive!) clause.


  /** Iterators */

  using iterator = Vector<BLit>::iterator;
  using const_iterator = Vector<BLit>::const_iterator;

  inline const_iterator begin() const noexcept { return _lits.cbegin(); }
  inline const_iterator end() const noexcept { return _lits.cend(); }
};

/**
 * CNF formula composed of clauses: C_1 /\ C_2 /\ ... /\ C_n.
 */
class CNFFormula
{
  Vector<CNFClause> _clauses; /**< list of clauses in the formula */

public:

  CNFFormula(); /**< construct a formula with no clauses (equivalent to true) */

  CNFFormula& operator&=(const CNFClause& clause); /**< add a new clause to this formula */
  CNFFormula& operator&=(const CNFFormula& other); /**< add all clauses in other formula to this formula */
  CNFFormula operator&(const CNFFormula& other) const; /**< create a new formula from the AND of two formulas */

  CNFClause& operator[](size_t i); /**< access clause by index */
  const CNFClause& operator[](size_t i) const; /**< read-only access */

  const Vector<CNFClause>& clauses() const;
  
  bool eval(const Set<BVar>& asgn) const; // DF 4/6/2018  given an assignment asgn, we return True iff the assignment evals true for all clauses in the CNF formula.

  CNFFormula projection(const Set<BVar>& vars) const; /**< remove from every clause the variables that are not in the set */

  /**
   * Returns graph where:
   * - Vertex v_i represents clause C_i;
   * - There is an edge between two vertices iff the clauses share a variable.
   */
  Graph<size_t> dualGraph() const;

  /** Iterators */

  using iterator = Vector<CNFClause>::iterator;
  using const_iterator = Vector<CNFClause>::const_iterator;

  inline const_iterator begin() const noexcept { return _clauses.cbegin(); }
  inline const_iterator end() const noexcept { return _clauses.cend(); }
};
