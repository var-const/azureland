#pragma once

#include "collision_typedefs.h"

#include <cinder/Vector.h>

#include <vector>

class GameObject;

struct Collision {
    GameObject* m_pObjA{};
    GameObject* m_pObjB{};
    float m_Time{};
};

struct CollisionsInfo {
#ifdef PASS_BY_VALUE
    CollisionsInfo(float Time, CollisionsIterT Begin, CollisionsIterT End);
#else
    CollisionsInfo(
        float Time, CollisionsIterT const& Begin, CollisionsIterT const& End);
#endif
    bool IsEmpty() const;
    int GetSize() const;

    float m_Time{};
    CollisionsIterT m_Begin;
    CollisionsIterT m_End;
};
