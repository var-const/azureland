#pragma once

#include "game_object.h"
#include "health_mixin.h"
#include "timer.h"
#include "weapons/reload.h"

#include <cinder/gl/Texture.h>

#include <vector>

class Player;

class Enemy : public GameObject, public HealthMixin {
  public:
    Enemy(Player& Player, const SizeT& Size, const PosT& StartPos);

    void SetSpeed(int Speed);

    void OnCollisionWithEnemy(const Enemy& Rhs);

    void LogicUpdate(float Dt) override;

    void SetParalyzed(int Milliseconds);
    void UnsetParalyzed();
    bool IsParalyzed() const;

  private:
    typedef std::vector<const Enemy*> BlockersContT;

    void UpdateAngle();

    void OnHealthDepleted() override;
    void TryRespawn();
    void Respawn(PosT Pos);

    void CheckBlocked();

    void OnCaughtPlayer();

    Player* m_pPlayer{};
    BlockersContT m_Blockers;
    int m_Sleep{};
    bool m_Blocked{};
    bool m_CheckBlocked{};

    float m_MaxPursuitRange{2000.f};

    bool m_NeedRespawn{};

    float m_Speed{};

    Reload m_Reload;

    int m_PointsForKilling{10};

    bool m_IsParalyzed{};
    CountdownTimer m_ParalyzedTimer;
};
