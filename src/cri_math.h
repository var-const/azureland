#pragma once

#include <limits>

bool IsFpointEq(float LHS, float RHS, float Eps =
    std::numeric_limits<float>::epsilon() );

template <typename T, typename std::enable_if_t<
    std::is_integral<T>::value>*
    = nullptr>
bool is_equal(T lhs, T rhs)
{
    return lhs == rhs;
}

template <typename T, typename std::enable_if_t<
    std::is_floating_point<T>::value>*
    = nullptr>
bool is_equal(T lhs, T rhs)
{
    return IsFpointEq(lhs, rhs);
}
