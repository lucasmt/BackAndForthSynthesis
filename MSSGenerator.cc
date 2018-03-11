
#include "MSSGenerator.hpp"

#include <limits>

using openwbo::MaxSATFormula;
using NSPACE::IntRange;
using NSPACE::IntOption;
using NSPACE::BoolOption;

/**
 * Solver options copied from open-wbo.
 */
const IntOption verbosity("Open-WBO", "verbosity",
			  "Verbosity level (0=minimal, 1=more).\n", 0,
			  IntRange(0, 1));

const IntOption weight("WBO", "weight-strategy",
		       "Weight strategy (0=none, 1=weight-based, 2=diversity-based).\n", 2,
		       IntRange(0, 2));

const BoolOption symmetry("WBO", "symmetry", "Symmetry breaking.\n", true);

const IntOption symmetry_lim("WBO", "symmetry-limit",
			     "Limit on the number of symmetry breaking clauses.\n", 500000,
			     IntRange(0, INT32_MAX));

/**
 * Add soft clause with the given weight to the formula.
 */
void addSoftClause(MaxSATFormula* formula,
		   uint64_t weight,
		   const CNFClause& clause)
{
  vec<Lit> lits;

  /* Convert literals to Glucose format */
  for (BLit lit : clause)
  {
    BVar var = abs(lit);

    while (var >= formula->nVars())
      formula->newVar();

    lits.push((lit > 0) ? mkLit(var) : ~mkLit(var));
  }

  /* Add clause to MaxSAT formula and update weights */
  formula->setMaximumWeight(weight);
  formula->updateSumWeights(weight);
  formula->addSoftClause(weight, lits);
}

/**
 * Add hard clause to the given formula. Hard clauses must always be satisfied.
 */
void addHardClause(MaxSATFormula* formula,
		   const CNFClause& clause)
{
  vec<Lit> lits;

  /* Convert literals to Glucose format */
  for (BLit lit : clause)
  {
    BVar var = abs(lit);

    while (var >= formula->nVars())
      formula->newVar();

    lits.push((lit > 0) ? mkLit(var) : ~mkLit(var));
  }

  formula->addHardClause(lits); /*< add clause to the formula */
}

/**
 * Add hard clause to the given formula conditioned on an indicator variable.
 * Example: if indicator is z_1 and clause is (y_1 \/ ~y_2 \/ y_3), the hard
 * clause (~z_1 \/ y_1 \/ ~y_2 \/ y_3) is added.
 */
void addHardClauseWithIndicator(MaxSATFormula* formula,
				BVar indicator,
				const CNFClause& clause)
{
  vec<Lit> lits;

  /* Convert literals to Glucose format */
  for (BLit lit : clause)
  {
    BVar var = abs(lit);

    while (var >= formula->nVars())
      formula->newVar();

    lits.push((lit > 0) ? mkLit(var) : ~mkLit(var));
  }

  lits.push(~mkLit(indicator)); /*< add indicator variable to clause*/

  formula->addHardClause(lits); /*< add clause to the formula */
}

MSSGenerator::MSSGenerator(const Vector<BVar>& indicators,
			   const Vector<CNFClause>& clauses)
  : maxSatSolver(verbosity, weight, symmetry, symmetry_lim)
{
  /* Set weight for hard clauses to the maximum possible value */
  uint64_t hardWeight = std::numeric_limits<uint64_t>::max();
  maxSatFormula.setHardWeight(hardWeight);

  for (size_t i = 0; i < clauses.size(); i++)
  {
    /* Add hard clause (z_i -> Y_i) */
    addHardClauseWithIndicator(&maxSatFormula, indicators[i], clauses[i]);
    
    /* Add soft clause (z_i) */
    addSoftClause(&maxSatFormula, 1, CNFClause(indicators[i]));
  }

  maxSatFormula.setProblemType(_UNWEIGHTED_);
  maxSatFormula.setFormat(_FORMAT_MAXSAT_);
}

void MSSGenerator::enforceClause(const CNFClause& clause)
{
  addHardClause(&maxSatFormula, clause);
}

Optional<Set<BVar>> MSSGenerator::generateMSS()
{
  maxSatSolver.loadFormula(&maxSatFormula);

  if (maxSatSolver.search()) /*< search was successful, return MSS */
  {
    Vector<BVar> model = maxSatSolver.getModel();

    return Set<BVar>(model.begin(), model.end());
  }
  else /*< we ran out of MSS, return null object */
  {
    return nullopt;
  }
}

Optional<Set<BVar>> MSSGenerator::generateMSSCovering(const Set<BVar>& vars)
{
  /* Create copy of the formula */
  MaxSATFormula* copy = maxSatFormula.copyMaxSATFormula();

  /* Add constraints enforcing that result covers the given set */
  for (BVar var : vars)
    addHardClause(copy, CNFClause(var));

  maxSatSolver.loadFormula(copy);

  if (maxSatSolver.search()) /*< search was successful, return MSS */
  {
    delete copy; /*< delete copy of the formula */

    Vector<BVar> model = maxSatSolver.getModel();

    return Set<BVar>(model.begin(), model.end());
  }
  else /*< we ran out of MSS, return null object */
  {
    delete copy; /*< delete copy of the formula */

    return nullopt;
  }
}
