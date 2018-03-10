#pragma once

#include "TrivialSpec.hpp"
#include "MSSSpec.hpp"
#include "Ref.hpp"

/**
 * Result of the CNF decomposition: (F_1, F_2).
 */
struct CNFChain
{
  const Ref<TrivialSpec> first; /**< F_1 */
  const Ref<MSSSpec> second; /**< F_2 */
};
