#pragma once

#include "cri_game_object.h"
#include "cri_health_mixin.h"
#include "weapons/cri_reload.h"

#include <vector>

class CRIPlayer;

class CRIEnemy : public CRIGameObject, public CRIHealthMixin
{
public:	
    CRIEnemy(CRIPlayer& Player, const SizeT& Size, const PosT& StartPos);
    
    void OnCollisionWithEnemy(const CRIEnemy& Rhs);

    void LogicUpdate(); // override

private:
    typedef std::vector<const CRIEnemy*> BlockersContT;
    typedef BlockersContT::iterator BlockersIterT;

    void OnHealthDepleted(); // override
    void TryRespawn();
    void Respawn(PosT Pos);

    void CheckBlocked();

    void OnCaughtPlayer();

    CRIPlayer* m_pPlayer;
    BlockersContT m_Blockers;
    int m_Sleep;
    bool m_Blocked;
    bool m_CheckBlocked;

    bool m_NeedRespawn;

    CRIReload m_Reload;
};
