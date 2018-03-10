#pragma once

#include "TrivialSpec.hpp"
#include "MSSSpec.hpp"

#include <memory>

/**
 * Type alias for smart pointer.
 */
using Ref<T> = std::shared_ptr<T>;
