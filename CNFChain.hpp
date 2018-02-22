#pragma once

#include "TrivialSpec.hpp"
#include "MSSSpec.hpp"

class CNFChain
{
  TrivialSpec _first;
  MSSSpec _second;
  
  public:

  CNFChain(TrivialSpec first, MSSSpec second);
  
  const TrivialSpec& first() const;
  const MSSSpec& second() const;
};
