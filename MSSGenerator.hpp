#pragma once

#include "CNFFormula.hpp"
#include "open-wbo/MaxSATFormula.h"
#include "open-wbo/algorithms/Alg_WBO.h"
#include <vector>

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

  void blockSubset(const std::vector<int>& vars);

  std::vector<int> generateMSS();

  std::vector<int> generateMSSCovering(const std::vector<int>& vars);
};
