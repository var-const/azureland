#include "stdafx.h"

#include "pickup_generator.h"

#include "game_scene.h"
#include "pickups.h"

#include <cinder/Rand.h>
#include <cinder/Vector.h>

using ci::Vec2f;

void PickupGenerator::Invoke(const Vec2f PosForPickUp)
{
    using ci::randBool;
    typedef Movable::SizeT SizeT;

    assert(m_pScene);

    if (randBool()) {
        m_pScene->AddObject(std::make_unique<HealthPickup>(SizeT(8.f, 8.f), PosForPickUp, 50, 10));
    }
}

void PickupGenerator::SetScene(GameScene& Scene) { m_pScene = &Scene; }
