#pragma once

#ifdef _DEBUG
#ifndef PERFORMANCE_METRICS
#define PERFORMANCE_METRICS
#endif
#endif

#include "collision.h"
#include "spatial_grid.h"

#ifdef PERFORMANCE_METRICS
#include <fstream>
#endif
#include <utility>
#include <vector>

namespace cinder {
template <typename T> class RectT;
template <typename T> class Vec2;
}

class GameObject;
struct CollisionsInfo;

class Collider {
  public:
    typedef std::vector<GameObject*> ObjContT;
    typedef ObjContT::iterator ObjIterT;
    typedef ObjContT::const_iterator ObjConstIterT;

    Collider(int Width, int Height);

    void Reserve(int Amount);

	template <typename IterT>
    CollisionsInfo BuildCollisions(IterT Begin, IterT End, float Time);

    ObjIterT CopyColliding(cinder::Vec2<int> LeftUpper,
        cinder::Vec2<int> RightLower, ObjIterT OutputIter) const;

    std::pair<bool, cinder::RectT<float>> GetEmptyCell(
        cinder::Vec2<int> RowLimits, cinder::Vec2<int> ColLimits) const;

  private:
    typedef SpatialGrid<30, 30> GridT;
    typedef std::pair<GameObject*, GameObject*> CheckT;
    typedef std::vector<CheckT> ChecksContT;
    typedef ChecksContT::iterator ChecksIterT;
    typedef ChecksContT::const_iterator ChecksConstIterT;

	template <typename IterT>
    void BroadPhase(IterT Begin, IterT End, float Time);
#ifdef PASS_BY_VALUE
    void AddChecks(GameObject* Obj, ObjConstIterT Begin, ObjConstIterT End);
#else
    void AddChecks(
        GameObject* Obj, ObjConstIterT const& Begin, ObjConstIterT const& End);
#endif
    void NarrowPhase(float Time);
    void TryAddCollision(GameObject& Lhs, GameObject& Rhs, float Time);

#ifdef PERFORMANCE_METRICS
    void OutputPerformanceMetrics(int ObjectsC);
#endif

    Collision::CollisionsContT m_CollisionsBuffer;
    Collision::CollisionsIterT m_CollisionsEndIter;
    ChecksContT m_Checks;
    ChecksIterT m_ChecksEndIter;
    float m_CurMinTime{};
    GridT m_Grid;

#ifdef PERFORMANCE_METRICS
    int m_ChecksC{};
    int m_CollisionsC{};
    int m_SimpleChecks{};
    int m_Duplicates{};
    std::ofstream m_PerformanceLog{"performance_log.txt"};
#endif
};
