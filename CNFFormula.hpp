#pragma once

#include "Vector.hpp"
#include "Set.hpp"
#include "Graph.hpp"

/**
 * Type aliases for representing CNF variables and literals.
 * A Var should always be positive.
 * A Lit can be either positive or negative.
 */
using Var = int;
using Lit = int;

/**
 * Clause in a CNF formula: (l_1 \/ l_2 \/ ... \/ l_k).
 */
class CNFClause
{
  Vector<Lit> _lits; /**< list of literals in the clause */

public:

  CNFClause(); /**< construct a clause with no literals (equivalent to false) */
  CNFClause(Lit l); /**< construct a clause with a single literal */
  CNFClause(Lit l1, Lit l2); /**< construct a clause with two literals */

  CNFClause& operator|=(Lit lit); /**< add a new literal to the clause */
  CNFClause& operator|=(const CNFClause& other); /**< add all literals in other clause to this clause */
  CNFClause operator|(const CNFClause& other) const; /**< create a new clause from the OR of two clauses */

  CNFClause projection(const Set<Var>& vars) const; /**< remove from clause all variables not in the set */

  /** Iterator definitions */
  using Vector<Lit>::iterator = iterator;
  using Vector<Lit>::const_iterator = const_iterator;

  inline iterator begin() noexcept { return vec.begin(); }
  inline const_iterator cbegin() const noexcept { return vec.cbegin(); }
  inline iterator end() noexcept { return vec.end(); }
  inline const_iterator cend() const noexcept { return vec.end(); }
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
  const CNFClause& operator[](size_t i) const; /** read-only access */

  CNFFormula projection(const Set<Var>& vars) const; /**< remove from every clause the variables that are not in the set */

  /**
   * Returns graph where:
   * - Vertex v_i represents clause C_i;
   * - There is an edge between two vertices iff the clauses share a variable.
   */
  Graph<size_t> dependencyGraph() const;

  /** Iterator definitions */
  using Vector<CNFClause>::iterator = iterator;
  using Vector<CNFClause>::const_iterator = const_iterator;

  inline iterator begin() noexcept { return vec.begin(); }
  inline const_iterator cbegin() const noexcept { return vec.cbegin(); }
  inline iterator end() noexcept { return vec.end(); }
  inline const_iterator cend() const noexcept { return vec.end(); }
};
