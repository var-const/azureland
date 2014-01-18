#pragma once

#include "cri_game_object.h"

#include <vector>

class CRIPlayer;

class CRIEnemy : public CRIGameObject
{
public:	
    CRIEnemy(CRIPlayer& Player, const SizeT& Size, const PosT& StartPos);
    
    void Collide(const CRIEnemy& Rhs);

    void LogicUpdate(); // override

private:
    typedef std::vector<const CRIEnemy*> BlockersContT;
    typedef BlockersContT::iterator BlockersIterT;

    void CheckBlocked();

    CRIPlayer* m_pPlayer;
    BlockersContT m_Blockers;
    int m_Sleep;
    bool m_Blocked;
    bool m_CheckBlocked;
};
