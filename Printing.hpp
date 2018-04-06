#pragma once

#include <iostream>

#include "CNFFormula.hpp"
#include "MSSSpec.hpp"
#include "TrivialSpec.hpp"
#include "CNFSpec.hpp"


/**
 * Prints a list of literals with the given separator between elements using the given variable name.
 * Example: If lits = [-1, 2, 3, -4], separator = ", " and varName = "x", prints "~x:1, x:2, x:3, ~x:4".
 */
void print(const Vector<BLit>& lits, const std::string& separator, const std::string& varName);

/**
 * Prints a set of variables in the form "{ x:i_1, x:i_2, ..., x:i_k }",
 * where i_1, ..., i_k are the variable identifiers and x is the given variable name.
 */
void print(const Set<BVar>& vars, const std::string& varName);

/**
 * Prints a vector of variables as a set in the form "{ x:i_1, x:i_2, ..., x:i_k }",
 * where i_1, ..., i_k are the variable identifiers and x is the given variable name.
 */
void print(const Vector<BVar>& vars, const std::string& varName);

/**
 * Prints the given clause.
 */
void print(const CNFClause& clause, const std::string& varName);

/**
 * Prints the given CNF clauses.
 */
void print(const Vector<CNFClause>& clauses, const std::string& varName);

/**
 * Prints the given CNF formula.
 */
void print(const CNFFormula& cnf, const std::string& varName);

/**
 * Prints the given CNF specification using the names given for the input and outpus variables.
 */
void print(const CNFSpec& spec, const std::string& inputVarName, const std::string& outputVarName);

/**
 * Prints the given specification of the form (z_1 <-> ~(l_1 | ... | l_k)) & ...,
 * where z_1 is an output variable and l_1, ..., l_k are input-variable literals.
 */
void print(const TrivialSpec& spec, const std::string& inputVarName, const std::string& outputVarName);

/**
 * Prints the given specification of the form (z_1 -> l_1 | ... | l_k) & ...,
 * where z_1 is an input variable and l_1, ..., l_k are output-variable literals.
 */
void print(const MSSSpec& spec, const std::string& inputVarName, const std::string& outputVarName);