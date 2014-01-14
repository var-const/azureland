#include "cri_stdafx.h"

#include "cri_collision_detail.h"

#include "cri_axis_getter.h"
#include "cri_collision.h"
#include "cri_movable.h"

namespace
{

typedef std::pair<float, float> TimeframeT; // @TODO: move out of here

} // unnamed

TimeframeT GetAxisCollisionTime( CRIMovable& Lhs, CRIMovable& Rhs,
    AxisGetter Axis );
TimeframeT GetTimeframe(TimeframeT A, TimeframeT B);

CRICollision CreateCollision( CRIGameObject& Lhs, CRIGameObject& Rhs,
    const float Time )
{
    // @TODO
    return CRICollision();
}

float GetCollisionTime( CRIMovable& Lhs, CRIMovable& Rhs )
{
    // For each axis separately, find the timeframe within which the objects
    // could have collided
    const TimeframeT Tx = GetAxisCollisionTime(Lhs, Rhs, AxisGetter::AxisX);
    const TimeframeT Ty = GetAxisCollisionTime(Lhs, Rhs, AxisGetter::AxisY);

    // Find if there was a timeframe during which there was an
    // intersection for both axes
    const TimeframeT Timeframe = GetTimeframe(Tx, Ty);
    return Timeframe.first; // @FIXME!
}

TimeframeT GetTimeframe(const TimeframeT A, const TimeframeT B)
{
    using std::max; using std::min;

    TimeframeT Result;
    Result.first = max(A.first, B.first);
    Result.second = min(A.second, B.second);
    if (Result.second <= Result.first)
    {
        Result.first = Result.second = -1.f; // @FIXME arbitrary value
    }
    return Result;
}

TimeframeT GetAxisCollisionTime( CRIMovable& Lhs, CRIMovable& Rhs,
    const AxisGetter Axis )
{
    using std::make_pair;
    typedef CRIMovable::BoxT BoxT;

    if ( Intersect(Lhs.GetAABB(), Rhs.GetAABB()) )
    {
        return TimeframeT(); // @FIXME
    }

    const BoxT Lbox = Lhs.GetAABB();
    const float Lmin = Axis.ThisAxis(GetLeftUpper(Lbox));
    const float Lmax = Axis.ThisAxis(GetRightLower(Lbox));

    const BoxT Rbox = Rhs.GetAABB();
    const float Rmin = Axis.ThisAxis(GetLeftUpper(Rbox));
    const float Rmax = Axis.ThisAxis(GetRightLower(Rbox));

    const float VDiff = Axis.ThisAxis(Lhs.GetVelocity()) -
        Axis.ThisAxis(Rhs.GetVelocity());

    float TEnter = -1.f;
    float TLeave = -1.f;
    if (VDiff > 0.f)
    {
        if (Rmax < Lmin)
        {
            TEnter = (Lmin - Rmax) / VDiff;
        }
        else if (Lmax > Rmin)
        {
            TLeave = (Lmax - Rmin) / VDiff;
        }
    }
    else
    {
        if (Lmax < Rmin)
        {
            TEnter = (Lmax - Rmin) / VDiff;
        }
        else if (Rmax > Lmin)
        {
            TLeave = (Lmin - Rmax) / VDiff;
        }
    }

    return make_pair(TEnter, TLeave);
}
