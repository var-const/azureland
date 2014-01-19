#include "cri_stdafx.h"

#include "cri_weapon.h"

CRIWeapon::~CRIWeapon()
{
}

void CRIWeapon::Shoot()
{
    assert(m_pCrosshair);

    const PosT PlayerPos = GetScene().ToScreenPos(GetCenterPos());
    VelT Dist = m_pCrosshair->GetCenterPos() - PlayerPos;
    Dist.safeNormalize();
    const VelT ProjSpeed = Dist * 800.f; // @FIXME hardcoded
    //const CRIProjectile* Projectile = new CRIProjectile(SizeT(10.f, 10.f),
    //    GetCenterPos(), ProjSpeed);
    //GetScene().AddObject(Projectile);
}
