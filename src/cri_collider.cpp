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

CRICollider::CRICollider(const int Width, const int Height)
: m_Grid(Width, Height)
, m_CurMinTime(0.f)
#ifdef PERFORMANCE_METRICS
, m_ChecksC(0)
, m_CollisionsC(0)
, m_SimpleChecks(0)
, m_Duplicates(0)
, m_PerformanceLog("performance_log")
, m_Hits()
#endif
{
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
        static_cast<float>(ObjectsC * ObjectsC) /
        static_cast<float>(m_ChecksC);
    m_PerformanceLog << "Better than brute force: ";
    m_PerformanceLog << ObjectsC * ObjectsC << " / " << m_ChecksC << " = " << BetterThanBruteForce << "\n";

    const volatile float LessThanPerfect =
        static_cast<float>(m_ChecksC) / static_cast<float>(m_CollisionsC);
    m_PerformanceLog << "Less than perfect: ";
    m_PerformanceLog << m_ChecksC << " / " << m_CollisionsC << " = " << LessThanPerfect << "\n";

    const volatile int ChecksSize = std::distance(m_Checks.begin(),
        m_ChecksEndIter);
    m_PerformanceLog << "Checks size: " << ChecksSize << "\n";

    const volatile float DuplicatesPercent =
        static_cast<float>(m_Duplicates) / static_cast<float>(ChecksSize);
    m_PerformanceLog << "Duplicates percent: ";
    m_PerformanceLog << m_Duplicates << " / " << ChecksSize << " = " << DuplicatesPercent << "\n";

    const volatile float SimpleChecksPercent =
        static_cast<float>(m_SimpleChecks) /
        static_cast<float>(m_SimpleChecks + m_ChecksC);
    m_PerformanceLog << "Simple checks percent: ";
    m_PerformanceLog << m_SimpleChecks << " / " << m_SimpleChecks + m_ChecksC << " = " << SimpleChecksPercent << "\n";
    m_PerformanceLog << m_Hits << "\n";
    m_PerformanceLog << "\n\n\n";

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
                if (Row == 0 && (*ObjIter)->GetYBounds().x < 0.f)
                {
                    ++m_Hits;
                }
                if (Row == GridT::RowsC - 1 && (*ObjIter)->GetYBounds().y > 1024.f * 3.f)
                {
                    ++m_Hits;
                }
                if (Col == 0 && (*ObjIter)->GetXBounds().x < 0.f)
                {
                    ++m_Hits;
                }
                if (Col == GridT::ColsC - 1 && (*ObjIter)->GetXBounds().y > 1280.f * 3.f)
                {
                    ++m_Hits;
                }
            }
        }
    }

    //sort(m_Checks.begin(), m_ChecksEndIter);
}

void CRICollider::AddChecks(CRIGameObject* const Obj, const ObjConstIterT Begin,
    const ObjConstIterT End)
{
    using std::make_pair;

    for (ObjConstIterT Iter = Begin; Iter != End; ++Iter)
    {
        *m_ChecksEndIter++ = CheckT(Obj, *Iter);
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
        if (LastObjA == i->m_Objects.first && LastObjB == i->m_Objects.second)
        {
#ifdef PERFORMANCE_METRICS
            ++m_Duplicates;
#endif
            continue;
        }
        LastObjA = i->m_Objects.first;
        LastObjB = i->m_Objects.second;

        TryAddCollision(*i->m_Objects.first, *i->m_Objects.second, Time);
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
    //if (m_CurMinTime > 1.f)
    //{
    //    m_CurMinTime = CollisionTime;
    //}
    //if (CollisionTime >= m_CurMinTime)
    //{
    //    m_CurMinTime = CollisionTime;
    //}
    //*m_CollisionsEndIter++ = CreateCollision(Lhs, Rhs, CollisionTime);
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
