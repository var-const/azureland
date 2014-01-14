#pragma once

#include "cri_collision_typedefs.h"

#include <cinder/Vector.h>

#include <vector>

class CRIGameObject;

struct CRICollision
{
    CRICollision();

    CRIGameObject* m_pObjA;
    CRIGameObject* m_pObjB;
    float m_Time;
    float m_Depth;
    ci::Vec2f m_Normal;
};

struct CRICollisionsInfo
{
    CRICollisionsInfo(float Time, CollisionsIterT Begin, CollisionsIterT End);
    bool IsEmpty() const;

    float m_Time;
    CollisionsIterT m_Begin;
    CollisionsIterT m_End;
};
