#pragma once

#include "math_vector.h"

// Allows for generic handling of both axes

struct AxisGetter
{
    enum Axes
    {
        AxisX,
        AxisY
    };

    AxisGetter(Axes OwnAxis);

    template <typename T> T& ThisAxis(Vec2<T>& p) const;
    template <typename T> const T& ThisAxis(const Vec2<T>& p) const;
    template <typename T> T& OtherAxis(Vec2<T>& p) const;
    template <typename T> const T& OtherAxis(const Vec2<T>& p) const;

    Axes m_OwnAxis;
};


//////////////////////////////////////////////////////////////////////////
// Impl
//////////////////////////////////////////////////////////////////////////

template <typename T> 
T& AxisGetter::ThisAxis(Vec2<T>& p) const
{
    return m_OwnAxis == AxisX ? p.x : p.y;
}

template <typename T>
const T& AxisGetter::ThisAxis(const Vec2<T>& p) const
{
    return m_OwnAxis == AxisX ? p.x : p.y;
}

template <typename T>
T& AxisGetter::OtherAxis(Vec2<T>& p) const
{
    return m_OwnAxis == AxisX ? p.y : p.x;
}

template <typename T>
const T& AxisGetter::OtherAxis(const Vec2<T>& p) const
{
    return m_OwnAxis == AxisX ? p.y : p.x;
}
