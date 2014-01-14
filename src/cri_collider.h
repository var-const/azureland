#pragma once

#include "cri_collision_typedefs.h"

#include <vector>

class CRIGameObject;
struct CRICollisionsInfo;

class CRICollider
{
public:
    typedef std::vector<CRIGameObject*> ObjContT;
    typedef ObjContT::iterator ObjIterT;

    CRICollider();

    void Reserve(int Amount);

    // Could be templated on iterator types, but I thought that would
    // be overkill
    CRICollisionsInfo BuildCollisions(ObjIterT Begin, ObjIterT End, float Time);

private:
    struct CmpCollisionTime
    {
        explicit CmpCollisionTime(float Time);
        bool operator()(const CRICollision& Other) const;
        float m_Time;
    };
    void BuildCollisions(CRIGameObject& Obj, ObjIterT Begin, ObjIterT End,
        float Time);
    void TryAddCollision(CRIGameObject& Lhs, CRIGameObject& Rhs, float Time);

    CollisionsContT m_CollisionsBuffer;
    float m_CurMinTime;
};
