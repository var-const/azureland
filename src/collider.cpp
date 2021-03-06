#include "stdafx.h"

#include "collider.h"

#include "collision.h"
#include "collision_detail.h"
#include "container_helpers.h"
#include "game_object.h"
#include "math_helpers.h"

#include <cinder/CinderMath.h>
#include <cinder/Rect.h>
#include <cinder/Vector.h>

#include <algorithm>
#include <cassert>
#include <cstddef>

using ci::Rectf;
using ci::Vec2f;
using ci::Vec2i;
using std::pair;

Collider::Collider(const int Width, const int Height)
  : m_Grid(Width, Height)
{
}

void Collider::Reserve(const int Amount)
{
    assert(Amount > 0);
    ResizeAtLeast(m_CollisionsBuffer, Amount * 2);
    ResizeAtLeast(m_Checks, Amount * 100);
}

template <typename IterT>
CollisionsInfo Collider::BuildCollisions(
    const IterT Begin, const IterT End, const float Time)
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
    OutputPerformanceMetrics(ObjectsC);
#endif

    return CollisionsInfo(
        m_CurMinTime, m_CollisionsBuffer.begin(), m_CollisionsEndIter);
}

template <typename IterT>
void Collider::BroadPhase(
    const IterT Begin, const IterT End, const float Time)
{
    m_Grid.Reinit(Begin, End, Time);
    m_ChecksEndIter = m_Checks.begin();

    for (std::size_t Row = 0U; Row != GridT::RowsC; ++Row) {
        for (std::size_t Col = 0U; Col != GridT::ColsC; ++Col) {
            const auto& Objects = m_Grid.m_Cells[Row][Col];
            const auto ObjEnd = Objects.end();
            for (auto ObjIter = Objects.begin(); ObjIter != ObjEnd; ++ObjIter) {
                AddChecks(*ObjIter, ObjIter + 1, ObjEnd);
            }
        }
    }
}

#ifdef PASS_BY_VALUE
void Collider::AddChecks(
    GameObject* const Obj, const ObjConstIterT Begin, const ObjConstIterT End)
#else
void Collider::AddChecks(
    GameObject* const Obj, ObjConstIterT const& Begin, ObjConstIterT const& End)
#endif
{
    using std::make_pair;

    for (auto Iter = Begin; Iter != End; ++Iter) {
        *m_ChecksEndIter++ =
            Obj < *Iter ? make_pair(Obj, *Iter) : make_pair(*Iter, Obj);
    }
}

void Collider::NarrowPhase(const float Time)
{
    using std::remove_if;

    m_CollisionsEndIter = m_CollisionsBuffer.begin();

    GameObject* LastObjA = nullptr;
    GameObject* LastObjB = nullptr;
    for (auto i = m_Checks.begin(); i != m_ChecksEndIter; ++i) {
        if (LastObjA == i->first && LastObjB == i->second) {
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
        m_CollisionsEndIter,
        [this](const Collision& other) { return other.m_Time > m_CurMinTime; });
}

void Collider::TryAddCollision(
    GameObject& Lhs, GameObject& Rhs, const float Time)
{
    if (!Intersect(Lhs.GetMovementAABB(), Rhs.GetMovementAABB())) {
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
    if (CollisionTime < 0.f || CollisionTime > Time) {
        return;
    }

#ifdef PERFORMANCE_METRICS
    ++m_CollisionsC;
#endif

    // @FIXME hack
    if (IsFpointEq(CollisionTime, 0.f) &&
        TouchOnly(Lhs.GetAABB(), Rhs.GetAABB())) {
        return;
    }
    if (CollisionTime <= m_CurMinTime) {
        m_CurMinTime = CollisionTime;
        *m_CollisionsEndIter++ = CreateCollision(Lhs, Rhs, CollisionTime);
    }
}

pair<bool, Rectf> Collider::GetEmptyCell(
    const Vec2i RowLimits, const Vec2i ColLimits) const
{
    using ci::math;
    using std::make_pair;
    using std::swap;

    int RowBegin = RowLimits.x >= 0 ? RowLimits.x : GridT::RowsC + RowLimits.x;
    int RowEnd = RowLimits.y >= 0 ? RowLimits.y : GridT::RowsC + RowLimits.y;
    if (RowBegin > RowEnd) {
        swap(RowBegin, RowEnd);
    }
    RowBegin = math<int>::clamp(RowBegin, 0, GridT::RowsC);
    RowEnd = math<int>::clamp(RowEnd, 0, GridT::RowsC);

    int ColBegin = ColLimits.x >= 0 ? ColLimits.x : GridT::ColsC + ColLimits.x;
    int ColEnd = ColLimits.y >= 0 ? ColLimits.y : GridT::ColsC + ColLimits.y;
    if (ColBegin > ColEnd) {
        swap(ColBegin, ColEnd);
    }
    ColBegin = math<int>::clamp(ColBegin, 0, GridT::ColsC);
    ColEnd = math<int>::clamp(ColEnd, 0, GridT::ColsC);

    for (int Row = RowBegin; Row != RowEnd; ++Row) {
        for (int Col = ColBegin; Col != ColEnd; ++Col) {
            if (m_Grid.m_Cells[Row][Col].empty()) {
                return make_pair(true, m_Grid.GetCellRect(Row, Col));
            }
        }
    }

    // A better way would be to make a more thorough search for empty
    // space, but the enemy who needs it can just wait, which is much simpler
    return make_pair(false, Rectf());
}

Collider::ObjIterT Collider::CopyColliding(
    const Vec2i LeftUpper, const Vec2i RightLower, ObjIterT OutputIter) const
{
    using ci::Vec2i;
    using std::max;
    using std::min;

    const Vec2i Begin = LeftUpper / m_Grid.GetCellSize();
    const Vec2i End = RightLower / m_Grid.GetCellSize();
    for (int Row = max(Begin.y, 0); Row < min(GridT::RowsC, End.y + 1); ++Row) {
        for (int Col = max(Begin.x, 0); Col < min(GridT::ColsC, End.x + 1);
             ++Col) {
            const ObjContT& Objects = m_Grid.m_Cells[Row][Col];
            OutputIter = copy(Objects.begin(), Objects.end(), OutputIter);
        }
    }

    return OutputIter;
}


#ifdef PERFORMANCE_METRICS
void Collider::OutputPerformanceMetrics(const int ObjectsC)
{
    const volatile float BetterThanBruteForce =
        static_cast<float>(ObjectsC * ObjectsC) / static_cast<float>(m_ChecksC);
    m_PerformanceLog << "Better than brute force: ";
    m_PerformanceLog << ObjectsC * ObjectsC << " / " << m_ChecksC << " = "
                     << BetterThanBruteForce << "\n";

    const volatile float LessThanPerfect =
        static_cast<float>(m_ChecksC) / static_cast<float>(m_CollisionsC);
    m_PerformanceLog << "Less than perfect: ";
    m_PerformanceLog << m_ChecksC << " / " << m_CollisionsC << " = "
                     << LessThanPerfect << "\n";

    const volatile int ChecksSize =
        std::distance(m_Checks.begin(), m_ChecksEndIter);
    m_PerformanceLog << "Checks size: " << ChecksSize << "\n";

    const volatile float DuplicatesPercent =
        static_cast<float>(m_Duplicates) / static_cast<float>(ChecksSize);
    m_PerformanceLog << "Duplicates percent: ";
    m_PerformanceLog << m_Duplicates << " / " << ChecksSize << " = "
                     << DuplicatesPercent << "\n";

    const volatile float SimpleChecksPercent =
        static_cast<float>(m_SimpleChecks) /
        static_cast<float>(m_SimpleChecks + m_ChecksC);
    m_PerformanceLog << "Simple checks percent: ";
    m_PerformanceLog << m_SimpleChecks << " / " << m_SimpleChecks + m_ChecksC
                     << " = " << SimpleChecksPercent << "\n";

    m_PerformanceLog << "\n\n\n";
}

#endif
