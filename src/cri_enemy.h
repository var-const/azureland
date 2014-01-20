#pragma once

#include "cri_game_object.h"
#include "cri_health_mixin.h"
#include "cri_timer.h"
#include "weapons/cri_reload.h"

#include <vector>
#include "cinder/gl/Texture.h"

class CRIPlayer;

class CRIEnemy : public CRIGameObject, public CRIHealthMixin
{
public:	
    CRIEnemy(CRIPlayer& Player, const SizeT& Size, const PosT& StartPos);

    void SetSpeed(int Speed);
    
    void OnCollisionWithEnemy(const CRIEnemy& Rhs);

    void LogicUpdate(float Dt); // override

    void SetParalyzed(int Milliseconds);
    void UnsetParalyzed();
    bool IsParalyzed() const;

private:
    typedef std::vector<const CRIEnemy*> BlockersContT;
    typedef BlockersContT::iterator BlockersIterT;
    void DoDraw();

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

    float m_MaxPursuitRange;

    bool m_NeedRespawn;

    float m_Speed;

    CRIReload m_Reload;

    int m_PointsForKilling;

    bool m_IsParalyzed;
    CRICountdownTimer m_ParalyzedTimer;
    ci::gl::Texture m_Tex;
};
