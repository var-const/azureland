#pragma once

#include "cri_game_object.h"
#include <set>

class CRIPlayer;

class CRIEnemy : public CRIGameObject
{
public:	
    CRIEnemy(CRIPlayer& Player, const SizeT& Size, const PosT& StartPos);
    
    void Collide(const CRIEnemy& Rhs);
private:
    void DoUpdate(float Dt); // override
    void CheckBlocked();

    CRIPlayer* m_pPlayer;
    CRI_AABB m_Target;
    int m_Sleep;
    std::set<const CRIEnemy*> m_Collide;
    int m_RecalcSleep;
    bool m_Blocked;
};
