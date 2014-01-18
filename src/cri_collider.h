#pragma once

#define PERFORMANCE_METRICS
#ifdef _DEBUG
#ifndef PERFORMANCE_METRICS
#define PERFORMANCE_METRICS
#endif
#endif

#include "cri_collision_typedefs.h"
#include "cri_spatial_grid.h"

#ifdef PERFORMANCE_METRICS
#include <fstream>
#endif
#include <vector>

namespace cinder { template <typename T> class Vec2; }

class CRIGameObject;
struct CRICollisionsInfo;

class CRICollider
{
public:
    typedef std::vector<CRIGameObject*> ObjContT;
    typedef ObjContT::iterator ObjIterT;
    typedef ObjContT::const_iterator ObjConstIterT;

    CRICollider(int Width, int Height);

    void Reserve(int Amount);

    // Could be templated on iterator types, but I thought that would
    // be overkill
    CRICollisionsInfo BuildCollisions(ObjIterT Begin, ObjIterT End, float Time);

private:
    struct Check
    {
        Check() : m_Hash(0) {}
        Check(CRIGameObject* A, CRIGameObject* B)
            : m_Objects(std::make_pair(A < B ? A : B, A < B ? B : A))
            , m_Hash(m_Objects.first >= m_Objects.second ?
            reinterpret_cast<int>(m_Objects.first) *
                reinterpret_cast<int>(m_Objects.first) + reinterpret_cast<int>(m_Objects.first) +
                reinterpret_cast<int>(m_Objects.second) :
                reinterpret_cast<int>(m_Objects.first) + reinterpret_cast<int>(m_Objects.second) *
                reinterpret_cast<int>(m_Objects.second))
            {}
        // a >= b ? a * a + a + b : a + b * b 
        std::pair<CRIGameObject*, CRIGameObject*> m_Objects;
        unsigned int m_Hash;
        bool operator < (Check Rhs) const { return m_Hash < Rhs.m_Hash; }
    };

    typedef CRISpatialGrid<30, 30> GridT;
    typedef Check CheckT;
    typedef std::vector<CheckT> ChecksContT;
    typedef ChecksContT::iterator ChecksIterT;
    typedef ChecksContT::const_iterator ChecksConstIterT;

    struct CmpCollisionTime
    {
        explicit CmpCollisionTime(float Time);
        bool operator()(const CRICollision& Other) const;
        float m_Time;
    };

    void BroadPhase(ObjIterT Begin, ObjIterT End, float Time);
    void AddChecks(CRIGameObject* Obj, ObjConstIterT Begin, ObjConstIterT End);
    void NarrowPhase(float Time);
    void BuildCollisionsWithObject(CRIGameObject& Obj, ObjConstIterT Begin,
        ObjConstIterT End, float Time);
    void TryAddCollision(CRIGameObject& Lhs, CRIGameObject& Rhs, float Time);

    CollisionsContT m_CollisionsBuffer;
    CollisionsIterT m_CollisionsEndIter;
    ChecksContT m_Checks;
    ChecksIterT m_ChecksEndIter;
    float m_CurMinTime;
    GridT m_Grid;

#ifdef PERFORMANCE_METRICS
    int m_ChecksC;
    int m_CollisionsC;
    int m_SimpleChecks;
    int m_Duplicates;
    int m_Hits;
    std::ofstream m_PerformanceLog;
#endif
};
