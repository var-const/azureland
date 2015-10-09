#include "stdafx.h"

#include "projectile.h"

Projectile::Projectile(
    const SizeT& Size, const PosT& Pos, const VelT& Velocity, const int Damage)
  : GameObject(Size, Pos)
  , m_Damage(Damage)
{
    SetVelocity(Velocity);
    SetTextureDescriptor(TextureDescr);
}

int Projectile::GetDamage() const { return m_Damage; }

int Projectile::TextureDescr = -1;
