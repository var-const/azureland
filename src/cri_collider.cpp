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
, m_CollisionsC(0)
, m_SimpleChecks(0)
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
    m_Pairs.reserve(static_cast<std::size_t>(Amount * 10));
}

CRICollisionsInfo CRICollider::BuildCollisions( const Vec2f SceneSize,
    const ObjIterT Begin, const ObjIterT End, const float Time )
{
    using ci::Vec2i;

    m_CollisionsBuffer.clear();
    m_CurMinTime = Time + 1.f;
#ifdef _DEBUG
    const int ObjectsC = std::distance(Begin, End);
    m_ChecksC = m_CollisionsC = 0;
    m_SimpleChecks = 0;
#endif

    m_Pairs.clear();

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
                for (ObjConstIterT Test = ObjIter + 1; Test != ObjEnd;
                    ++Test)
                {
                    m_Pairs.push_back(*ObjIter < *Test ? std::make_pair(*ObjIter, *Test) :
                        std::make_pair(*Test, *ObjIter));
                //BuildCollisionsWithObject(**ObjIter, ObjIter + 1, ObjEnd, Time);
                }
            }
        }
    }
    std::sort(m_Pairs.begin(), m_Pairs.end());
    CRIGameObject *a = 0, *b = 0;
#ifdef _DEBUG
    int duplicates = 0;
#endif
    for (auto i = m_Pairs.begin(); i != m_Pairs.end(); ++i)
    {
        if (a == i->first && b == i->second)
        {
#ifdef _DEBUG
            ++duplicates;
#endif
            continue;
        }
        a = i->first;
        b = i->second;
        TryAddCollision(*i->first, *i->second, Time);
    }
    const CollisionsIterT Last = remove_if(m_CollisionsBuffer.begin(),
        m_CollisionsBuffer.end(), CmpCollisionTime(m_CurMinTime));
    m_CollisionsBuffer.erase(Last, m_CollisionsBuffer.end());

#ifdef _DEBUG
    const volatile float BetterThanBruteForce =
        static_cast<float>(ObjectsC * (ObjectsC - 1)) /
        static_cast<float>(m_ChecksC);
    const volatile float LessThanPerfect =
        static_cast<float>(m_ChecksC) / static_cast<float>(m_CollisionsC);
    const volatile float DuplicatesPercent =
        static_cast<float>(duplicates) / static_cast<float>(m_Pairs.size());
    const volatile float SimpleChecksPercent =
        static_cast<float>(m_SimpleChecks) / static_cast<float>(m_SimpleChecks + m_ChecksC);
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
    if ( !Intersect(Lhs.GetMovementAABB(), Rhs.GetMovementAABB()) )
    {
#ifdef _DEBUG
        ++m_SimpleChecks;
        const float CollisionTime = GetCollisionTime(Lhs, Rhs, Time);
        assert(CollisionTime < 0.f || CollisionTime > Time);
#endif
        return;
    }
#ifdef _DEBUG
    ++m_ChecksC;
#endif

    const float CollisionTime = GetCollisionTime(Lhs, Rhs, Time);
    if (CollisionTime < 0.f || CollisionTime > Time)
    {
        return;
    }

#ifdef _DEBUG
    ++m_CollisionsC;
#endif

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
