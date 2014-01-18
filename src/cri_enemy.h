#pragma once

#include "cri_game_object.h"
#include <set>

class CRIPlayer;

class CRIEnemy : public CRIGameObject
{
public:	
    CRIEnemy(CRIPlayer& Player, const SizeT& Size, const PosT& StartPos);
    
    void Collide(const CRIEnemy& Rhs);

    void LogicUpdate(); // override

private:
    void CheckBlocked();

    CRIPlayer* m_pPlayer;
    CRI_AABB m_Target;
    std::set<const CRIEnemy*> m_Collide;
    int m_Sleep;
    bool m_Blocked;
};
