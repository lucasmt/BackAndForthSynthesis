#pragma once

#include <set>
#include <vector>

class VarSet
{
  std::set<int> _vars;

public:

  VarSet();

  VarSet(std::vector<int>::const_iterator begin,
        std::vector<int>::const_iterator end);

  const std::set<int>& vars() const; 

  void insert(int v);

  void insert(std::vector<int>::const_iterator begin,
	      std::vector<int>::const_iterator end);

  int max() const;

  bool isElem(int v) const;

  bool subsetOfAny(const std::vector<VarSet>& supersets) const;

  void print() const;

  VarSet setDifference(const VarSet& other) const;
};
