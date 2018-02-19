#include "CNFChain.hpp"

#include <utility>

using std::move;

CNFChain::CNFChain(TrivialSpec first, MSSSpec second)
  : _first(move(first))
  , _second(move(second))
{}

const TrivialSpec& CNFChain::first() const
{
    return _first;
}

const MSSSpec& CNFChain::second() const
{
    return _second;
}
