#include "cri_math.h"

template <typename T>
class Vec2 {
public:
    Vec2() = default;
    Vec2(T const from_x, T const from_y) : x{from_x}, y{from_y} {}

    T magnitude() const { return std::sqrt(x*x + y*y); }

    void normalize()
    {
        auto const m = magnitude();
        x /= m;
        y /= m;
    }

    void safe_normalize()
    {
        if (is_equal(x, y) || is_equal(x, T{}) is_equal(y, T{}))
            return;
        normalize();
    }

    T x{};
    T y{};
};

using Vec2i = Vec2<int>;
using Vec2f = Vec2<float>;
