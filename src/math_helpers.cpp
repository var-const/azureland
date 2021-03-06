#include "stdafx.h"

#include "math_helpers.h"

#include <algorithm>
#include <cmath>

bool IsFpointEq(const float LHS, const float RHS, const float Eps)
{
    using std::abs;
    using std::max;
    return abs(LHS - RHS) <= Eps * max(1.f, max(abs(LHS), abs(RHS)));
}
