#include "VarSet.hpp"

#include <algorithm>

using std::set;
using std::vector;

VarSet::VarSet() {}

VarSet::VarSet(vector<int>::const_iterator begin,
	       vector<int>::const_iterator end)
: _vars(begin, end) {}

void VarSet::insert(int v)
{
  _vars.insert(v);
}

void VarSet::insert(vector<int>::const_iterator begin,
		    vector<int>::const_iterator end)
{
  _vars.insert(begin, end);
}

const set<int>& VarSet::vars() const
{
  return _vars;
}

int VarSet::max() const
{
  return *max_element(_vars.begin(), _vars.end());
}

bool VarSet::isElem(int v) const
{
  return _vars.find(v) != _vars.end();
}

bool VarSet::subsetOfAny(const vector<VarSet>& supersets) const
{
  for (const VarSet& superset : supersets)
  {
    bool isSubset = includes(superset._vars.begin(), superset._vars.end(),
			     _vars.begin(), _vars.end());
   
    if (isSubset)
      return true;
  }

  return false;
}
