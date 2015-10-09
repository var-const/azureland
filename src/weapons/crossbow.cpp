#include "stdafx.h"

#include "crossbow.h"

#include "weapon_helpers.h"
#include "../cri_game_scene.h"
#include "../cri_projectile.h"

#include <cinder/Vector.h>

using ci::Vec2f;

void Crossbow::DoShoot(const Vec2f StartPos, const Vec2f TargetPos)
{
    typedef Movable::SizeT SizeT;

    Projectile* const Projectile = new Projectile(SizeT(10.f, 10.f), StartPos,
        800.f * GetTargetVector(StartPos, TargetPos), 10);
    GetScene().AddObject(*Projectile);
}
