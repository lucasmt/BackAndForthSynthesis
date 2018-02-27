#pragma once

#include "CNFFormula.hpp"
#include "open-wbo/MaxSATFormula.h"
#include "open-wbo/algorithms/Alg_WBO.h"
#include <vector>
#include <set>

class MSSGenerator
{
  openwbo::MaxSATFormula maxSatFormula;
  openwbo::WBO solver;

  void addSoftClause(uint64_t weight,
		     const CNFClause& clause,
		     openwbo::MaxSATFormula* formula);

  void addHardClause(const CNFClause& clause,
		     openwbo::MaxSATFormula* formula);

  void addHardClauseWithIndicator(const CNFClause& clause,
				  int indicator,
				  openwbo::MaxSATFormula* formula);

public:

  MSSGenerator(const std::vector<CNFClause>& clauses,
	       const std::vector<int>& indicators);

  void blockSubset(const std::set<int>& vars);

  bool generateMSS();

  bool generateMSSCovering(const std::set<int>& vars);

  VarSet getMSS() const;
};
