#include "stdafx.h"

#include "forcefield_emitter.h"

#include "forcefield.h"
#include "../game_scene.h"

#include <cinder/Vector.h>

using ci::Vec2f;

ForcefieldEmitter::ForcefieldEmitter(const Player& Player)
  : m_pPlayer(&Player)
{
}

void ForcefieldEmitter::SetPlayer(const Player& Player) { m_pPlayer = &Player; }

void ForcefieldEmitter::DoShoot(const Vec2f StartPos, const Vec2f TargetPos)
{
    typedef Movable::SizeT SizeT;

    assert(m_pPlayer);
    auto Field = std::make_unique<Forcefield>(200.f, StartPos, 300, 10, 500.f, *m_pPlayer);
    GetScene().AddObject(std::move(Field));
}
