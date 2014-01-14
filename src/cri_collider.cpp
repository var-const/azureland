#include "cri_stdafx.h"

#include "cri_collider.h"

#include "cri_collision.h"
#include "cri_collision_detail.h"
#include "cri_game_object.h"

#include <algorithm>
#include <cassert>
#include <cstddef>

CRICollider::CRICollider()
: m_CurMinTime(0.f)
{
}

void CRICollider::Reserve( const int Amount )
{
    assert(Amount > 0);
    m_CollisionsBuffer.reserve(static_cast<std::size_t>(Amount));
}

CRICollisionsInfo CRICollider::BuildCollisions( const ObjIterT Begin,
    const ObjIterT End, const float Time )
{
    // @FIXME naive approach (O(n * (n - 1)))
    for (ObjIterT i = Begin; i != End; ++i)
    {
        BuildCollisions(**i, i + 1, End, Time);
    }
    return CRICollisionsInfo(m_CurMinTime, m_CollisionsBuffer.begin(),
        m_CollisionsBuffer.end());
}

void CRICollider::BuildCollisions( CRIGameObject& Obj, const ObjIterT Begin,
    const ObjIterT End, const float Time )
{
    using std::remove_if;

    m_CurMinTime = Time + 1.f;
    for (ObjIterT i = Begin; i != End; ++i)
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
    const float CollisionTime = GetCollisionTime(Lhs, Rhs);
    if (CollisionTime < 0.f || CollisionTime > Time)
    {
        return;
    }
    if (CollisionTime <= m_CurMinTime)
    {
        m_CurMinTime = CollisionTime;
        m_CollisionsBuffer.push_back(CreateCollision(Lhs, Rhs, Time));
    }
}

CRICollider::CmpCollisionTime::CmpCollisionTime( const float Time )
: m_Time(Time)
{
}

bool CRICollider::CmpCollisionTime::operator()( const CRICollision& Other )
    const
{
    return Other.Time > m_Time;
}
