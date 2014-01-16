#include "cri_stdafx.h"

#include "cri_collider.h"

#include "cri_collision.h"
#include "cri_collision_detail.h"
#include "cri_game_object.h"
#include "cri_math.h"

#include <cinder/Vector.h>

#include <algorithm>
#include <cassert>
#include <cstddef>

using ci::Vec2f;

CRICollider::CRICollider()
: m_CurMinTime(0.f)
#ifdef _DEBUG
, m_ChecksC(0)
#endif
{
    using ci::Vec2f; using ci::Vec2i;
    //m_GridParams.SceneSize = Vec2f(1280.f, 1024.f); // @FIXME
    //m_GridParams.CellsCount = Vec2i(8, 8);
    //m_GridParams.CellsCount = Vec2i(13, 13);
    //m_GridParams.CellsCount = Vec2i(15, 15); // 13
    //m_GridParams.CellsCount = Vec2i(16, 16); // 8-10 cpu, 40% reinit
    //m_GridParams.CellsCount = Vec2i(17, 17); // 8-10 cpu, 44% reinit
    //m_GridParams.CellsCount = Vec2i(18, 18); // 9-10 cpu, 49% reinit
    //m_GridParams.CellsCount = Vec2i(19, 19); // 9-10
    //m_GridParams.CellsCount = Vec2i(20, 20); // 9-11
    //m_GridParams.CellsCount = Vec2i(22, 22); // 13
    //m_Grid.SetParams(m_GridParams);
    m_Grid.SetSize(Vec2i(1280, 1024));
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

    m_CollisionsBuffer.clear();
    m_CurMinTime = Time + 1.f;
#ifdef _DEBUG
    const int ObjectsC = std::distance(Begin, End);
    m_ChecksC = 0;
#endif

    m_Grid.Reinit(Begin, End, Time);
    for (int Row = 0; Row != GridT::RowsC; ++Row)
    {
        for (int Col = 0; Col != GridT::ColsC; ++Col)
        {
            const ObjContT& Objects = m_Grid.m_Cells[Row][Col];
            const ObjConstIterT ObjEnd = Objects.end();
            for (ObjConstIterT ObjIter = Objects.begin(); ObjIter != ObjEnd;
                ++ObjIter)
            {
                BuildCollisionsWithObject(**ObjIter, ObjIter + 1, ObjEnd, Time);
            }
        }
    }

#ifdef _DEBUG
    const volatile float BetterThanBruteForce =
        static_cast<float>(ObjectsC * (ObjectsC - 1)) /
        static_cast<float>(m_ChecksC);
#endif

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
#ifdef _DEBUG
    ++m_ChecksC;
#endif

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
