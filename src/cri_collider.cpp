#include "cri_stdafx.h"

#include "cri_collider.h"

#include "cri_collision.h"
#include "cri_collision_detail.h"
#include "cri_game_object.h"
#include "cri_math.h"
#include "cri_spatial_grid.h"

#include <cinder/Vector.h>

#include <algorithm>
#include <cassert>
#include <cstddef>

using ci::Vec2f;

CRICollider::CRICollider()
: m_CurMinTime(0.f)
//#ifdef _DEBUG
, m_ChecksC(0)
//#endif
{
}

void CRICollider::Reserve( const int Amount )
{
    assert(Amount > 0);
    m_CollisionsBuffer.reserve(static_cast<std::size_t>(Amount));
}

CRICollisionsInfo CRICollider::BuildCollisions( const Vec2f SceneSize,
    const ObjIterT Begin, const ObjIterT End, const float Time )
{
    using ci::Vec2i;
    typedef CRISpatialGrid GridT;

    m_CollisionsBuffer.clear();
    m_CurMinTime = Time + 1.f;
//#ifdef _DEBUG
    const int ObjectsC = std::distance(Begin, End);
    m_ChecksC = 0;
//#endif

    GridT::Parameters Params;
    Params.SceneSize = SceneSize;
    Params.CellsCount = Vec2i(8, 8);
    const GridT Grid = GridT(Begin, End, Time, Params);
    for (GridT::CellIterT cell = Grid.CellsBegin(); cell != Grid.CellsEnd();
        ++cell)
    {
        const ObjConstIterT ObjEnd = cell->m_Objects.end();
        for (ObjConstIterT i = cell->m_Objects.begin(); i != ObjEnd; ++i)
        {
            BuildCollisionsWithObject(**i, i + 1, ObjEnd, Time);
        }
    }

//#ifdef _DEBUG
    const volatile float BetterThanBruteForce =
        static_cast<float>(ObjectsC * (ObjectsC - 1)) /
        static_cast<float>(m_ChecksC);
//#endif

    return CRICollisionsInfo(m_CurMinTime, m_CollisionsBuffer.begin(),
        m_CollisionsBuffer.end());
}

void CRICollider::BuildCollisionsWithObject( CRIGameObject& Obj,
    const ObjConstIterT Begin, const ObjConstIterT End, const float Time )
{
    using std::remove_if;

    for (ObjConstIterT i = Begin; i != End; ++i)
    {
        TryAddCollision(Obj, **i, Time);
    }
    const CollisionsIterT Last = remove_if(m_CollisionsBuffer.begin(),
        m_CollisionsBuffer.end(), CmpCollisionTime(m_CurMinTime));
    m_CollisionsBuffer.erase(Last, m_CollisionsBuffer.end());
}

void CRICollider::TryAddCollision( CRIGameObject& Lhs, CRIGameObject& Rhs,
    const float Time )
{
//#ifdef _DEBUG
    ++m_ChecksC;
//#endif

    const float CollisionTime = GetCollisionTime(Lhs, Rhs, Time);
    if (CollisionTime < 0.f || CollisionTime > Time)
    {
        return;
    }
    // @FIXME hack
    if ( IsFpointEq(CollisionTime, 0.f) &&
        TouchOnly(Lhs.GetAABB(), Rhs.GetAABB()) )
    {
        return;
    }
    if (CollisionTime <= m_CurMinTime)
    {
        m_CurMinTime = CollisionTime;
        m_CollisionsBuffer.push_back(CreateCollision(Lhs, Rhs, CollisionTime));
    }
}

CRICollider::CmpCollisionTime::CmpCollisionTime( const float Time )
: m_Time(Time)
{
}

bool CRICollider::CmpCollisionTime::operator()( const CRICollision& Other )
    const
{
    return Other.m_Time > m_Time;
}
