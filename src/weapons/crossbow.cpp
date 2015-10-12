#include "stdafx.h"

#include "crossbow.h"

#include "weapon_helpers.h"
#include "../game_scene.h"
#include "../projectile.h"

#include <cinder/Vector.h>
#include <memory>

using ci::Vec2f;

void Crossbow::DoShoot(const Vec2f StartPos, const Vec2f TargetPos)
{
    typedef Movable::SizeT SizeT;

    auto projectile = std::make_unique<Projectile>(SizeT(10.f, 10.f), StartPos,
        800.f * GetTargetVector(StartPos, TargetPos), 10);
    GetScene().AddObject(std::move(projectile));
}
