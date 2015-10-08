#pragma once

#include <cinder/Vector.h>

// Allows for generic handling of both axes

struct AxisGetter
{
    enum Axes
    {
        AxisX,
        AxisY
    };

    AxisGetter(Axes OwnAxis);

    template <typename T> T& ThisAxis(ci::Vec2<T>& p) const;
    template <typename T> const T& ThisAxis(const ci::Vec2<T>& p) const;
    template <typename T> T& OtherAxis(ci::Vec2<T>& p) const;
    template <typename T> const T& OtherAxis(const ci::Vec2<T>& p) const;

    Axes m_OwnAxis;
};


//////////////////////////////////////////////////////////////////////////
// Impl
//////////////////////////////////////////////////////////////////////////

template <typename T> 
T& AxisGetter::ThisAxis(ci::Vec2<T>& p) const
{
    return m_OwnAxis == AxisX ? p.x : p.y;
}

template <typename T>
const T& AxisGetter::ThisAxis(const ci::Vec2<T>& p) const
{
    return m_OwnAxis == AxisX ? p.x : p.y;
}

template <typename T>
T& AxisGetter::OtherAxis(ci::Vec2<T>& p) const
{
    return m_OwnAxis == AxisX ? p.y : p.x;
}

template <typename T>
const T& AxisGetter::OtherAxis(const ci::Vec2<T>& p) const
{
    return m_OwnAxis == AxisX ? p.y : p.x;
}
