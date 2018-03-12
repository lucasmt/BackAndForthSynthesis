#pragma once

#include <set>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <cstddef>

/**
 * Type alias for std::set and additional operations.
 */
template <class T>
using Set = std::set<T>;

/* Implementation of templated functions in header file so that it can be accessed by code instantiating the template. */

template <class T>
T maxElement(const Set<T>& set)
{
  return *set.rbegin();
}

template <class T>
bool isSubset(const Set<T>& subset, const Set<T>& superset)
{
  return std::includes(superset.begin(), superset.begin(),
		       subset.begin(), subset.end());
}

template <class T>
Set<T> setUnion(const Set<T>& set1, const Set<T>& set2)
{
  Set<T> newSet;

  std::set_union(set1.begin(), set1.end(),
		 set2.begin(), set2.end(),
		 std::inserter(newSet, newSet.begin()));

  return newSet;
}

template <class T>
Set<T> setDifference(const Set<T>& set1, const Set<T>& set2)
{
  Set<T> newSet;

  std::set_difference(set1.begin(), set1.end(),
		      set2.begin(), set2.end(),
		      std::inserter(newSet, newSet.begin()));

  return newSet;
}

/** Returns the set representing the range [fromInclusive, toInclusive]. */
Set<std::size_t> range(std::size_t fromInclusive, std::size_t toInclusive);
