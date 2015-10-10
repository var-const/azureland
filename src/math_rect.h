#pragma once

#include "math_vector.h"

template <typename T>
class Rect {
public:
    Rect() = default;
    Rect(Vec2<T> const left_upper, Vec2<T> const right_lower) : left_upper_{left_upper},
        right_lower_{right_lower} {}

    // @TODO: snake_case
    // @TODO: make free functions
    Vec2<T> getSize() const { return {get_x2() - get_x1(), get_y2() - get_y1()}; }
    Vec2<T> get_center() const { return Vec2<T>{get_x1() + get_x2(), get_y1() + get_y2()} / T{2}; }
    T get_x1() const { return left_upper_.x; }
    T get_y1() const { return left_upper_.y; }
    T get_x2() const { return right_lower_.x; }
    T get_y2() const { return right_lower_.y; }

private:
    Vec2<T> left_upper_;
    Vec2<T> right_lower_;
};

using Rectf = Rect<float>;
