#pragma once

#include "cri_collision_typedefs.h"

#include <cinder/Vector.h>

#include <vector>

class CRIGameObject;

struct CRICollision
{
    CRIGameObject* ObjA;
    CRIGameObject* ObjB;
    float Depth;
    ci::Vec2f Normal;
};

struct CRICollisionsInfo
{
    bool IsEmpty() const;
    float m_Time;
    CollisionsIterT m_Begin;
    CollisionsIterT m_End;
};
