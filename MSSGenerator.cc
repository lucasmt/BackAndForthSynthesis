#include "MSSGenerator.hpp"

using std::vector;
using std::set;
using openwbo::MaxSATFormula;
using NSPACE::IntRange;
using NSPACE::IntOption;
using NSPACE::BoolOption;

MSSGenerator::MSSGenerator(const vector<CNFClause>& clauses,
			   const vector<int>& indicators)
  : solver(IntOption("Open-WBO", "verbosity",
		     "Verbosity level (0=minimal, 1=more).\n", 0,
		     IntRange(0, 1)),
	   IntOption("WBO", "weight-strategy",
		     "Weight strategy (0=none, 1=weight-based, 2=diversity-based).\n", 2,
		     IntRange(0, 2)),
	   BoolOption("WBO", "symmetry", "Symmetry breaking.\n", true),
	   IntOption("WBO", "symmetry-limit",
		     "Limit on the number of symmetry breaking clauses.\n", 500000,
		     IntRange(0, INT32_MAX)))
{
  int hardWeight = clauses.size() + 1;
  maxSatFormula.setHardWeight(hardWeight);

  for (size_t i = 0; i < clauses.size(); i++)
  {
    addHardClauseWithIndicator(clauses[i], indicators[i], &maxSatFormula);
    addSoftClause(1, CNFClause(indicators[i]), &maxSatFormula);
  }

  maxSatFormula.setProblemType(_UNWEIGHTED_);
  maxSatFormula.setFormat(_FORMAT_MAXSAT_);
}

void MSSGenerator::addSoftClause(uint64_t weight,
				 const CNFClause& clause,
				 MaxSATFormula* formula)
{
  vec<Lit> lits;

  for (int l : clause.lits())
  {
    int var = abs(l);

    while (var >= formula->nVars())
      formula->newVar();

    lits.push((l > 0) ? mkLit(var) : ~mkLit(var));
  }

  formula->setMaximumWeight(weight);
  formula->updateSumWeights(weight);
  formula->addSoftClause(weight, lits);
}

void MSSGenerator::addHardClause(const CNFClause& clause,
				 MaxSATFormula* formula)
{
  vec<Lit> lits;

  for (int l : clause.lits())
  {
    int var = abs(l);

    while (var >= formula->nVars())
      formula->newVar();

    lits.push((l > 0) ? mkLit(var) : ~mkLit(var));
  }

  formula->addHardClause(lits);
}

void MSSGenerator::addHardClauseWithIndicator(const CNFClause& clause,
					      int indicator,
					      MaxSATFormula* formula)
{
  vec<Lit> lits;

  for (int l : clause.lits())
  {
    int var = abs(l);

    while (var >= formula->nVars())
      formula->newVar();

    lits.push((l > 0) ? mkLit(var) : ~mkLit(var));
  }

  lits.push(~mkLit(indicator));

  formula->addHardClause(lits);
}

void MSSGenerator::blockSubset(const set<int>& vars)
{
  vec<Lit> lits;

  for (int var : vars)
    lits.push(~mkLit(var));

  maxSatFormula.addHardClause(lits);
}

bool MSSGenerator::generateMSS()
{
  solver.loadFormula(&maxSatFormula);

  return solver.search();
}

bool MSSGenerator::generateMSSCovering(const set<int>& vars)
{
  MaxSATFormula* copy = maxSatFormula.copyMaxSATFormula();

  for (int var : vars)
    addHardClause(CNFClause(var), copy);

  solver.loadFormula(copy);

  return solver.search();
}

VarSet MSSGenerator::getMSS() const
{
  vector<int> model = solver.getModel();

  return VarSet(model.begin(), model.end());
}

/*
vector<int> MSSGenerator::generateMSSCovering(const vector<int>& vars)
{
  MaxSATFormula copy = maxSatFormula;
  uint64_t harderWeight = copy.getHardWeight();

  for (int var : vars)
    addSoftClause(harderWeight, CNFClause(var));

  copy.setHardWeight(copy.getMaximumWeight() + 1);

  copy.search();

  return copy.model();
}
*/
