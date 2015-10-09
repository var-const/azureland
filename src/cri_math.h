#pragma once

#include <limits>

bool IsFpointEq(
    float LHS, float RHS, float Eps = std::numeric_limits<float>::epsilon());
