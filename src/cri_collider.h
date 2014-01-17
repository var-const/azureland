#pragma once

#ifdef _DEBUG
#ifndef PERFORMANCE_METRICS
#define PERFORMANCE_METRICS
#endif
#endif

#include "cri_collision_typedefs.h"
#include "cri_spatial_grid.h"

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

    CRICollider();

    void Reserve(int Amount);

    // Could be templated on iterator types, but I thought that would
    // be overkill
    CRICollisionsInfo BuildCollisions(ObjIterT Begin, ObjIterT End, float Time);

private:
    typedef CRISpatialGrid<30, 30> GridT;
    typedef std::pair<CRIGameObject*, CRIGameObject*> CheckT;
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
#endif
};
