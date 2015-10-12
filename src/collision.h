#pragma once

#include <cinder/Vector.h>

#include <vector>

class GameObject;

struct Collision {
	typedef std::vector<Collision> CollisionsContT;
	typedef CollisionsContT::iterator CollisionsIterT;

    GameObject* m_pObjA{};
    GameObject* m_pObjB{};
    float m_Time{};
};

struct CollisionsInfo {
#ifdef PASS_BY_VALUE
    CollisionsInfo(float Time, Collision::CollisionsIterT Begin, Collision::CollisionsIterT End);
#else
    CollisionsInfo(
        float Time, Collision::CollisionsIterT const& Begin, Collision::CollisionsIterT const& End);
#endif
    bool IsEmpty() const;
    int GetSize() const;
	auto begin() const { return m_Begin; }
	auto end() const { return m_End; }

    float m_Time{};
    Collision::CollisionsIterT m_Begin;
    Collision::CollisionsIterT m_End;
};
