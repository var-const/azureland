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
    Vec2<T> get_upper_left() const { return left_upper_; }
    Vec2<T> get_lower_right() const { return right_lower_; }
    void include(const Vec2<T>& point)
    {
        left_upper_ = {std::min(left_upper_.x, point.x), std::min(left_upper_.y, point.y) };
        right_lower_ = {std::max(right_lower_.x, point.x), std::max(right_lower_.y, point.y) };
    }
    template<typename U>
    void include(const Rect<U>& rhs)
    {
        include(rhs.get_upper_left());
        include(rhs.get_lower_right());
    }

private:
    Vec2<T> left_upper_;
    Vec2<T> right_lower_;
};

using Rectf = Rect<float>;
