#include "cri_stdafx.h"

#include "cri_collision_detail.h"

#include "cri_axis_getter.h"
#include "cri_collision.h"
#include "cri_math.h"
#include "cri_movable.h"

namespace
{

typedef std::pair<float, float> TimeframeT; // @TODO: move out of here

} // unnamed

TimeframeT GetAxisCollisionTime( CRIMovable& Lhs, CRIMovable& Rhs,
    AxisGetter Axis, float Time );
TimeframeT GetTimeframe(TimeframeT A, TimeframeT B);

CRICollision CreateCollision( CRIGameObject& Lhs, CRIGameObject& Rhs,
    const float Time )
{
    CRICollision Result;
    Result.m_pObjA = &Lhs;
    Result.m_pObjB = &Rhs;
    Result.m_Time = Time;
    // @TODO: depth, normal
    return Result;
}

float GetCollisionTime( CRIMovable& Lhs, CRIMovable& Rhs, const float Time )
{
    // For each axis separately, find the timeframe within which the objects
    // could have collided
    const TimeframeT Tx = GetAxisCollisionTime(Lhs, Rhs, AxisGetter::AxisX, Time);
    const TimeframeT Ty = GetAxisCollisionTime(Lhs, Rhs, AxisGetter::AxisY, Time);

    // Find if there was a timeframe during which there was an
    // intersection for both axes
    const TimeframeT Timeframe = GetTimeframe(Tx, Ty);
    if (Timeframe.first > 0.f)
    {
        return Timeframe.first;
    }
    else
    {
        return Timeframe.second > 0.f ? 0.f : -1.f;
    }
}

TimeframeT GetTimeframe(const TimeframeT A, const TimeframeT B)
{
    using std::max; using std::min;

    TimeframeT Result;
    Result.first = max(A.first, B.first);
    Result.second = min(A.second, B.second);
    if (Result.second < Result.first)
    {
        Result.first = Result.second = -1.f; // @FIXME arbitrary value
    }
    return Result;
}

TimeframeT GetAxisCollisionTime( CRIMovable& Lhs, CRIMovable& Rhs,
    const AxisGetter Axis, const float Time )
{
    using std::make_pair; using std::max; using std::min;
    typedef CRIMovable::BoxT BoxT;

    const BoxT Lbox = Lhs.GetAABB();
    const float Lmin = Axis.ThisAxis(GetLeftUpper(Lbox));
    const float Lmax = Axis.ThisAxis(GetRightLower(Lbox));

    const BoxT Rbox = Rhs.GetAABB();
    const float Rmin = Axis.ThisAxis(GetLeftUpper(Rbox));
    const float Rmax = Axis.ThisAxis(GetRightLower(Rbox));

    // @FIXME
    using std::abs;
    const bool AreIntersecting = abs(Axis.ThisAxis(Lbox.m_Center) -
        Axis.ThisAxis(Rbox.m_Center)) < Axis.ThisAxis(Lbox.m_HalfSize) +
        Axis.ThisAxis(Rbox.m_HalfSize);
    if (AreIntersecting)
    {
        //return TimeframeT(); // @FIXME
    }

    const float VDiff = Axis.ThisAxis(Lhs.GetVelocity()) -
        Axis.ThisAxis(Rhs.GetVelocity());
    if (IsFpointEq(VDiff, 0.f))
    {
        if (AreIntersecting)
        {
            return TimeframeT(0.f, Time);
        }
        else
        {
            // If they were not intersecting to begin with, they never would
            return TimeframeT(-1.f, -1.f);
        }
    }

    const float TEnter = (Rmin - Lmax) / VDiff;
    const float TLeave = (Rmax - Lmin) / VDiff;

    return make_pair(min(TEnter, TLeave), max(TEnter, TLeave));
}
