#pragma once

#include <set>

/**
 * Generic class for ordered sets.
 */
template <class T>
class Set
{
  std::set<T> _elems;

public:

  Set();

  void insert(T elem);

  void insert(const Set<T>& other);

  T max() const;

  bool hasElem(T elem) const;

  bool subsetOf(const Set<T>& other) const;

  Set<T> setUnion(const Set<T>& other) const;

  Set<T> setDifference(const Set<T>& other) const;

  /* Iterators */
  
  typedef typename std::set<T>::iterator iterator;
  typedef typename std::set<T>::const_iterator iterator;

  inline iterator begin() noexcept;
  inline const_iterator cbegin() const noexcept;
  inline iterator end() noexcept;
  inline const_iterator cend() const noexcept;
};
