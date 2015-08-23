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

template <typename T>
T clamp(T const what, T const min_val, T const max_val)
{
    if (what < min_val)
        return min_val;
    return what < max_val ? what : max_val;
}
