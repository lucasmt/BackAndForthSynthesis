#pragma once

#include "Vector.hpp"

#include <set>

/**
 * Extension of std::set.
 */
template <class T>
using Set = std::set<T>;

template <class T>
T maxElement(const Set<T>& set);

template <class T>
bool isSubset(const Set<T>& set1, const Set<T>& set2);

template <class T>
Set<T> setUnion(const Set<T>& set1, const Set<T>& set2);

template <class T>
Set<T> setDifference(const Set<T>& set1, const Set<T>& set2);
