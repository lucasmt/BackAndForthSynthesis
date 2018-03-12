#pragma once

#include <vector>
#include <set>
#include <cstddef>

/**
 * Type alias for vectors and additional operations.
 */
template<class T>
using Vector = std::vector<T>;

/**
 * Extracts the subsequence given by the set of indices.
 */
template<class T>
Vector<T> subsequence(const Vector<T>& sequence, const std::set<std::size_t>& indices)
{
  Vector<T> sub;

  for (size_t i : indices)
    sub.push_back(sequence[i]);

  return sub;
}
