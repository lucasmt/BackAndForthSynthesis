#include "CNFChain.hpp"

#include <utility>

using std::move;

CNFChain::CNFChain(TrivialSpec fst, MSSSpec snd)
  : first(move(first))
  , second(move(second))
{}
