#pragma once

#include "game_object.h"

#include <vector>

class Enemy;
class Player;

class Forcefield : public GameObject {
  public:
    Forcefield(const float Radius, const PosT& Pos, int Time, int Damage,
        float PushForce, const Player& Player);

    bool Affect(const Enemy& Enemy);
    int GetDamage() const;

    void LogicUpdate(float Dt) override;
    ci::Vec2f GetPushVector(ci::Vec2f ToPos) const;

    static int TextureDescr;

  private:
    typedef std::vector<const Enemy*> AffectedContT;

    void DoDraw() override;

    float m_TimeLeft{};
    float m_EffectTime{};
    int m_Damage{};
    float m_PushForce{};
    float m_TargetRadius{};

    AffectedContT m_Affected;
    const Player* m_pPlayer{};
};
