#pragma once

#include <unordered_map>

/**
 * Generic class for unordered maps.
 */
template <class K, class V>
class Map
{
  std::unordered_map<K, V> _entries;

public:

  Map();

  V& operator[](const K& key);

  V at(const K& key) const;

  /* Iterators */
  
  typedef typename std::unordered_map<K, V>::iterator iterator;
  typedef typename std::unordered_map<K, V>::const_iterator iterator;

  inline iterator begin() noexcept;
  inline const_iterator cbegin() const noexcept;
  inline iterator end() noexcept;
  inline const_iterator cend() const noexcept;
};
