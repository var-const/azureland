#include "cri_stdafx.h"

#include "cri_projectile.h"

CRIProjectile::CRIProjectile( const SizeT& Size, const PosT& Pos,
    const VelT& Velocity, const int Damage )
: CRIGameObject(Size, Pos)
, m_Damage(Damage)
{
    SetVelocity(Velocity);
}

int CRIProjectile::GetDamage() const
{
    return m_Damage;
}
