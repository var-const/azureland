#include "stdafx.h"

#include "collision_detail.h"

#include "axis_getter.h"
#include "collision.h"
#include "math.h"
#include "movable.h"

namespace {

typedef std::pair<float, float> TimeframeT; // @TODO: move out of here

} // unnamed

#ifdef PASS_BY_VALUE
TimeframeT GetTimeframe(TimeframeT A, TimeframeT B);
#else
TimeframeT GetTimeframe(TimeframeT const& A, TimeframeT const& B);
#endif

Collision CreateCollision(
    GameObject& Lhs, GameObject& Rhs, const float Time)
{
    Collision Result;

    Result.m_pObjA = &Lhs;
    Result.m_pObjB = &Rhs;
    Result.m_Time = Time;

    return Result;
}

float GetCollisionTime(Movable& Lhs, Movable& Rhs, const float Time)
{
    using ci::Vec2f;
    using std::abs;
    using std::make_pair;
    using std::max;
    using std::min;
    typedef Movable::BoxT BoxT;

    Vec2f TEnter;
    Vec2f TLeave;

    const Vec2f VDiff = Lhs.GetVelocity() - Rhs.GetVelocity();
    if (VDiff.x < 0.001f) {
        const bool AreIntersecting =
            abs(Lhs.GetCenterPos().x - Rhs.GetCenterPos().x) <
            Lhs.GetHalfSize().x + Rhs.GetHalfSize().x;
        if (AreIntersecting) {
            TEnter.x = 0.f;
            TLeave.x = Time;
        }
        else {
            // If they were not intersecting to begin with, they never would
            return -1.f;
        }
    }
    else {
        const Vec2f LBounds = Lhs.GetXBounds();
        const Vec2f RBounds = Rhs.GetXBounds();
        TEnter.x = (RBounds.x - LBounds.y) / VDiff.x;
        TLeave.x = (RBounds.y - LBounds.x) / VDiff.x;
    }

    if (VDiff.y < 0.001f) {
        const bool AreIntersecting =
            abs(Lhs.GetCenterPos().y - Rhs.GetCenterPos().y) <
            Lhs.GetHalfSize().y + Rhs.GetHalfSize().y;
        if (AreIntersecting) {
            TEnter.y = 0.f;
            TLeave.y = Time;
        }
        else {
            return -1.f;
        }
    }
    else {
        const Vec2f LBounds = Lhs.GetYBounds();
        const Vec2f RBounds = Rhs.GetYBounds();
        TEnter.y = (RBounds.x - LBounds.y) / VDiff.y;
        TLeave.y = (RBounds.y - LBounds.x) / VDiff.y;
    }


    const TimeframeT Tx =
        make_pair(min(TEnter.x, TLeave.x), max(TEnter.x, TLeave.x));
    const TimeframeT Ty =
        make_pair(min(TEnter.y, TLeave.y), max(TEnter.y, TLeave.y));
    // Find if there was a timeframe during which there was an
    // intersection for both axes
    const TimeframeT Timeframe = GetTimeframe(Tx, Ty);
    if (Timeframe.first > 0.f) {
        return Timeframe.first;
    }
    else {
        return Timeframe.second > 0.f ? 0.f : -1.f;
    }
}

#ifdef PASS_BY_VALUE
TimeframeT GetTimeframe(const TimeframeT A, const TimeframeT B)
#else
TimeframeT GetTimeframe(TimeframeT const& A, TimeframeT const& B)
#endif
{
    using std::max;
    using std::min;

    TimeframeT Result;
    Result.first = max(A.first, B.first);
    Result.second = min(A.second, B.second);
    if (Result.second < Result.first) {
        Result.first = Result.second = -1.f; // @FIXME arbitrary value
    }
    return Result;
}
