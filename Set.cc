#include "Set.hpp"

using std::size_t;
using std::iota;
using std::vector;

Set<size_t> range(size_t fromInclusive, size_t toInclusive)
{
  vector<size_t> r(toInclusive - fromInclusive + 1);

  iota(r.begin(), r.end(), fromInclusive);

  return Set<size_t>(r.begin(), r.end());
}
