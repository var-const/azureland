#pragma once

#include "cri_collision_typedefs.h"

#include <cinder/Vector.h>

#include <vector>

class CRIGameObject;

struct CRICollision
{
    CRICollision();

    CRIGameObject* ObjA;
    CRIGameObject* ObjB;
    float Time;
    float Depth;
    ci::Vec2f Normal;
};

struct CRICollisionsInfo
{
    CRICollisionsInfo(float Time, CollisionsIterT Begin, CollisionsIterT End);
    bool IsEmpty() const;

    float m_Time;
    CollisionsIterT m_Begin;
    CollisionsIterT m_End;
};
