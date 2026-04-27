#pragma once

#include "Entity.h"

#include <string>

namespace tactics {

bool loadInitialCondition(const std::string& filepath, InitialCondition& out);

}  // namespace tactics
