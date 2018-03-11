#pragma once

#include "TrivialSpec.hpp"
#include "MSSSpec.hpp"

/**
 * Result of the CNF decomposition: (F_1, F_2).
 */
struct CNFChain
{
  const TrivialSpec first; /**< F_1 */
  const MSSSpec second; /**< F_2 */

  CNFChain(TrivialSpec fst, MSSSpec snd);
};
