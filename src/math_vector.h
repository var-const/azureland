#pragma once

#include "cri_math.h"
#include <SDL2pp/Point.hh>
#include <cmath>

template <typename T>
class Vec2 {
public:
    Vec2() = default;
    Vec2(const SDL2pp::Point& p) : x{p.GetX()}, y{p.GetY()} {}
    Vec2(T const from_x, T const from_y) : x{from_x}, y{from_y} {}
    template <typename U>
    Vec2(Vec2<U> const from) : x(from.x), y(from.y) {}

    T magnitude() const { return std::sqrt(x*x + y*y); }

    void normalize()
    {
        auto const m = magnitude();
        x /= m;
        y /= m;
    }

    void safe_normalize()
    {
        if (is_equal(x, y) || is_equal(x, T{}) || is_equal(y, T{}))
            return;
        normalize();
    }

    T x{};
    T y{};
};

template <typename T, typename U>
Vec2<T> operator+(Vec2<T> const lhs, Vec2<U> const rhs)
    { return {lhs.x + rhs.x, lhs.y + rhs.y}; }
template <typename T, typename U>
Vec2<T> operator-(Vec2<T> const lhs, Vec2<U> const rhs)
    { return {lhs.x - rhs.x, lhs.y - rhs.y}; }
template <typename T, typename U>
Vec2<T> operator*(Vec2<T> const vec, U const scalar)
    { return {vec.x * scalar, vec.y * scalar}; }
template <typename T, typename U>
Vec2<T> operator/(Vec2<T> const vec, U const scalar)
    { return {vec.x / scalar, vec.y / scalar}; }

using Vec2i = Vec2<int>;
using Vec2f = Vec2<float>;
