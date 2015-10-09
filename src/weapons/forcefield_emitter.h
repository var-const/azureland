#pragma once

#include "weapon.h"

namespace cinder {
template <typename T> class Vec2;
}

class Player;

class ForcefieldEmitter : public Weapon {
  public:
    explicit ForcefieldEmitter(const Player& Player);

    void SetPlayer(const Player& Player);

  private:
    void DoShoot(
        cinder::Vec2<float> StartPos, cinder::Vec2<float> TargetPos) override;

    const Player* m_pPlayer{};
};
