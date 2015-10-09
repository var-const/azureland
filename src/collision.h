#pragma once

#include "cri_collision_typedefs.h"

#include <cinder/Vector.h>

#include <vector>

class CRIGameObject;

struct CRICollision {
    CRIGameObject* m_pObjA{};
    CRIGameObject* m_pObjB{};
    float m_Time{};
};

struct CRICollisionsInfo {
#ifdef PASS_BY_VALUE
    CRICollisionsInfo(float Time, CollisionsIterT Begin, CollisionsIterT End);
#else
    CRICollisionsInfo(
        float Time, CollisionsIterT const& Begin, CollisionsIterT const& End);
#endif
    bool IsEmpty() const;
    int GetSize() const;

    float m_Time{};
    CollisionsIterT m_Begin;
    CollisionsIterT m_End;
};
