#include "cri_stdafx.h"

#include "cri_collider.h"

#include "cri_collision.h"
#include "cri_collision_detail.h"
#include "cri_container_helpers.h"
#include "cri_game_object.h"
#include "cri_math.h"

#include <cinder/Vector.h>

#include <algorithm>
#include <cassert>
#include <cstddef>

using ci::Vec2f;

CRICollider::CRICollider()
: m_CurMinTime(0.f)
#ifdef PERFORMANCE_METRICS
, m_ChecksC(0)
, m_CollisionsC(0)
, m_SimpleChecks(0)
, m_Duplicates(0)
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
    ResizeAtLeast(m_CollisionsBuffer, Amount * 2);
    ResizeAtLeast(m_Checks, Amount * 100);
}

CRICollisionsInfo CRICollider::BuildCollisions( const ObjIterT Begin,
    const ObjIterT End, const float Time )
{
    using ci::Vec2i;
    using std::make_pair;

    m_CurMinTime = Time + 1.f;
#ifdef PERFORMANCE_METRICS
    const int ObjectsC = std::distance(Begin, End);
    m_ChecksC = m_CollisionsC = m_SimpleChecks = 0;
    m_Duplicates = 0;
#endif

    BroadPhase(Begin, End, Time);
    NarrowPhase(Time);

#ifdef PERFORMANCE_METRICS
    const volatile float BetterThanBruteForce =
        static_cast<float>(ObjectsC * (ObjectsC - 1)) /
        static_cast<float>(m_ChecksC);
    const volatile float LessThanPerfect =
        static_cast<float>(m_ChecksC) / static_cast<float>(m_CollisionsC);
    const volatile int ChecksSize = std::distance(m_Checks.begin(),
        m_ChecksEndIter);
    const volatile float DuplicatesPercent =
        static_cast<float>(m_Duplicates) / static_cast<float>(ChecksSize);
    const volatile float SimpleChecksPercent =
        static_cast<float>(m_SimpleChecks) /
        static_cast<float>(m_SimpleChecks + m_ChecksC);
    const volatile int Collisions = std::distance(m_CollisionsBuffer.begin(),
        m_CollisionsEndIter);
#endif

    return CRICollisionsInfo(m_CurMinTime, m_CollisionsBuffer.begin(),
        m_CollisionsEndIter);
}

void CRICollider::BroadPhase( const ObjIterT Begin, const ObjIterT End,
    const float Time )
{
    using std::sort;

    m_Grid.Reinit(Begin, End, Time);
    m_ChecksEndIter = m_Checks.begin();

    for (std::size_t Row = 0U; Row != GridT::RowsC; ++Row)
    {
        for (std::size_t Col = 0U; Col != GridT::ColsC; ++Col)
        {
            const ObjContT& Objects = m_Grid.m_Cells[Row][Col];
            const ObjConstIterT ObjEnd = Objects.end();
            for (ObjConstIterT ObjIter = Objects.begin(); ObjIter != ObjEnd;
                ++ObjIter)
            {
                AddChecks(*ObjIter, ObjIter + 1, ObjEnd);
            }
        }
    }

    sort(m_Checks.begin(), m_ChecksEndIter);
} 

void CRICollider::AddChecks(CRIGameObject* const Obj, const ObjConstIterT Begin,
    const ObjConstIterT End)
{
    using std::make_pair;

    for (ObjConstIterT Iter = Begin; Iter != End; ++Iter)
    {
        *m_ChecksEndIter++ = Obj < *Iter ? make_pair(Obj, *Iter) :
            make_pair(*Iter, Obj);
    }
}

void CRICollider::NarrowPhase(const float Time)
{
    using std::remove_if;

    m_CollisionsEndIter = m_CollisionsBuffer.begin();

    CRIGameObject* LastObjA = NULL;
    CRIGameObject* LastObjB = NULL;
    for (ChecksConstIterT i = m_Checks.begin(); i != m_ChecksEndIter; ++i)
    {
        if (LastObjA == i->first && LastObjB == i->second)
        {
#ifdef PERFORMANCE_METRICS
            ++m_Duplicates;
#endif
            continue;
        }
        LastObjA = i->first;
        LastObjB = i->second;

        TryAddCollision(*i->first, *i->second, Time);
    }

    m_CollisionsEndIter = remove_if(m_CollisionsBuffer.begin(),
        m_CollisionsEndIter, CmpCollisionTime(m_CurMinTime));
}

void CRICollider::TryAddCollision( CRIGameObject& Lhs, CRIGameObject& Rhs,
    const float Time )
{
    if ( !Intersect(Lhs.GetMovementAABB(), Rhs.GetMovementAABB()) )
    {
#ifdef PERFORMANCE_METRICS
        ++m_SimpleChecks;
#endif
#ifdef _DEBUG
        const float CollisionTime = GetCollisionTime(Lhs, Rhs, Time);
        assert(CollisionTime < 0.f || CollisionTime > Time);
#endif
        return;
    }
#ifdef PERFORMANCE_METRICS
    ++m_ChecksC;
#endif

    const float CollisionTime = GetCollisionTime(Lhs, Rhs, Time);
    if (CollisionTime < 0.f || CollisionTime > Time)
    {
        return;
    }

#ifdef PERFORMANCE_METRICS
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
        *m_CollisionsEndIter++ = CreateCollision(Lhs, Rhs, CollisionTime);
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
