#include "cri_stdafx.h"

#include "cri_crossbow.h"

#include "cri_weapon_helpers.h"
#include "../cri_game_scene.h"
#include "../cri_projectile.h"

#include <cinder/Vector.h>

using ci::Vec2f;

void CRICrossbow::DoShoot(const Vec2f StartPos, const Vec2f TargetPos)
{
    typedef CRIMovable::SizeT SizeT;

    CRIProjectile* const Projectile = new CRIProjectile(SizeT(10.f, 10.f),
        StartPos, 800.f * GetTargetVector(StartPos, TargetPos), 10);
    GetScene().AddObject(*Projectile);
}
