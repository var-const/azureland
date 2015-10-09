#pragma once

#include "game_object.h"

class Projectile : public GameObject {
  public:
    Projectile(
        const SizeT& Size, const PosT& Pos, const VelT& Velocity, int Damage);

    int GetDamage() const;

    static int TextureDescr;

  private:
    int m_Damage{};
};
