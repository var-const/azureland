#pragma once

#include "weapon.h"

namespace cinder {
template <typename T> class Vec2;
}

class Crossbow : public Weapon {
  private:
    void DoShoot(
        cinder::Vec2<float> StartPos, cinder::Vec2<float> TargetPos) override;
};
