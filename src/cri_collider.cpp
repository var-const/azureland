#include "cri_stdafx.h"

#include "cri_collider.h"

#include "cri_collision.h"
#include "cri_collision_detail.h"
#include "cri_container_helpers.h"
#include "cri_game_object.h"
#include "cri_math.h"

#include "math_rect.h"
#include "math_vector.h"

#include <algorithm>
#include <cassert>
#include <cstddef>

using std::pair;

CRICollider::CRICollider(const int Width, const int Height)
: m_Grid(Width, Height)
, m_CurMinTime(0.f)
#ifdef PERFORMANCE_METRICS
, m_ChecksC(0)
, m_CollisionsC(0)
, m_SimpleChecks(0)
, m_Duplicates(0)
, m_PerformanceLog("performance_log.txt")
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
    OutputPerformanceMetrics(ObjectsC);
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
}

#ifdef PASS_BY_VALUE
void CRICollider::AddChecks(CRIGameObject* const Obj, const ObjConstIterT Begin,
    const ObjConstIterT End)
#else
void CRICollider::AddChecks(CRIGameObject* const Obj, ObjConstIterT const& Begin,
    ObjConstIterT const& End)
#endif
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

pair<bool, Rectf > CRICollider::GetEmptyCell( const Vec2i RowLimits,
    const Vec2i ColLimits ) const
{
    using ci::math;
    using std::make_pair; using std::swap;

    int RowBegin = RowLimits.x >= 0 ? RowLimits.x : GridT::RowsC + RowLimits.x;
    int RowEnd = RowLimits.y >= 0 ? RowLimits.y : GridT::RowsC + RowLimits.y;
    if (RowBegin > RowEnd)
    {
        swap(RowBegin, RowEnd);
    }
    RowBegin = clamp(RowBegin, 0, GridT::RowsC);
    RowEnd = clamp(RowEnd, 0, GridT::RowsC);

    int ColBegin = ColLimits.x >= 0 ? ColLimits.x : GridT::ColsC + ColLimits.x;
    int ColEnd = ColLimits.y >= 0 ? ColLimits.y : GridT::ColsC + ColLimits.y;
    if (ColBegin > ColEnd)
    {
        swap(ColBegin, ColEnd);
    }
    ColBegin = clamp(ColBegin, 0, GridT::ColsC);
    ColEnd = clamp(ColEnd, 0, GridT::ColsC);

    for (int Row = RowBegin; Row != RowEnd; ++Row)
    {
        for (int Col = ColBegin; Col != ColEnd; ++Col)
        {
            if (m_Grid.m_Cells[Row][Col].empty())
            {
                return make_pair(true, m_Grid.GetCellRect(Row, Col));
            }
        }
    }

    // A better way would be to make a more thorough search for empty
    // space, but the enemy who needs it can just wait, which is much simpler
    return make_pair(false, Rectf());
}

CRICollider::ObjIterT CRICollider::CopyColliding( const Vec2i LeftUpper,
    const Vec2i RightLower, ObjIterT OutputIter ) const
{
    
    using std::max; using std::min;

    const Vec2i Begin = LeftUpper / m_Grid.GetCellSize();
    const Vec2i End = RightLower / m_Grid.GetCellSize();
    for (int Row = max(Begin.y, 0); Row < min(GridT::RowsC, End.y + 1);
        ++Row)
    {
        for (int Col = max(Begin.x, 0); Col < min(GridT::ColsC, End.x + 1);
            ++Col)
        {
            const ObjContT& Objects = m_Grid.m_Cells[Row][Col];
            OutputIter = copy(Objects.begin(), Objects.end(), OutputIter);
        }
    }

    return OutputIter;
}


#ifdef PERFORMANCE_METRICS
void CRICollider::OutputPerformanceMetrics(const int ObjectsC)
{
    const volatile float BetterThanBruteForce =
        static_cast<float>(ObjectsC * ObjectsC) /
        static_cast<float>(m_ChecksC);
    m_PerformanceLog << "Better than brute force: ";
    m_PerformanceLog << ObjectsC * ObjectsC << " / " << m_ChecksC << " = " <<
        BetterThanBruteForce << "\n";

    const volatile float LessThanPerfect =
        static_cast<float>(m_ChecksC) / static_cast<float>(m_CollisionsC);
    m_PerformanceLog << "Less than perfect: ";
    m_PerformanceLog << m_ChecksC << " / " << m_CollisionsC << " = " <<
        LessThanPerfect << "\n";

    const volatile int ChecksSize = std::distance(m_Checks.begin(),
        m_ChecksEndIter);
    m_PerformanceLog << "Checks size: " << ChecksSize << "\n";

    const volatile float DuplicatesPercent =
        static_cast<float>(m_Duplicates) / static_cast<float>(ChecksSize);
    m_PerformanceLog << "Duplicates percent: ";
    m_PerformanceLog << m_Duplicates << " / " << ChecksSize << " = " <<
        DuplicatesPercent << "\n";

    const volatile float SimpleChecksPercent =
        static_cast<float>(m_SimpleChecks) /
        static_cast<float>(m_SimpleChecks + m_ChecksC);
    m_PerformanceLog << "Simple checks percent: ";
    m_PerformanceLog << m_SimpleChecks << " / " << m_SimpleChecks + m_ChecksC <<
        " = " << SimpleChecksPercent << "\n";

    m_PerformanceLog << "\n\n\n";
}

#endif

CRICollider::CmpCollisionTime::CmpCollisionTime( const float Time )
: m_Time(Time)
{
}

bool CRICollider::CmpCollisionTime::operator()( const CRICollision& Other )
    const
{
    return Other.m_Time > m_Time;
}
