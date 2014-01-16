#pragma once

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
    CRICollisionsInfo BuildCollisions(cinder::Vec2<float> SceneSize,
        ObjIterT Begin, ObjIterT End, float Time);

private:
    typedef CRISpatialGrid<30, 30> GridT;

    struct CmpCollisionTime
    {
        explicit CmpCollisionTime(float Time);
        bool operator()(const CRICollision& Other) const;
        float m_Time;
    };

    void BuildCollisionsWithObject(CRIGameObject& Obj, ObjConstIterT Begin,
        ObjConstIterT End, float Time);
    void TryAddCollision(CRIGameObject& Lhs, CRIGameObject& Rhs, float Time);

    CollisionsContT m_CollisionsBuffer;
    float m_CurMinTime;
    GridT m_Grid;

#ifdef _DEBUG
    int m_ChecksC;
#endif
};
