#include "cri_stdafx.h"

#include "cri_projectile.h"

CRIProjectile::CRIProjectile( const SizeT& Size, const PosT& Pos,
    const VelT& Velocity, const int Damage )
: CRIGameObject(Size, Pos)
, m_Damage(Damage)
{
    SetVelocity(Velocity);
    SetTextureDescriptor(TextureDescr);
}

int CRIProjectile::GetDamage() const
{
    return m_Damage;
}

int CRIProjectile::TextureDescr = -1;
