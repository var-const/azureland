#include "stdafx.h"

#include "pickup_generator.h"

#include "game_scene.h"
#include "pickups.h"

#include <cinder/Rand.h>
#include <cinder/Vector.h>

using ci::Vec2f;

void CRIPickupGenerator::Invoke(const Vec2f PosForPickUp)
{
    using ci::randBool;
    typedef CRIMovable::SizeT SizeT;

    assert(m_pScene);

    if (randBool()) {
        CRIHealthPickup* const Pickup =
            new CRIHealthPickup(SizeT(8.f, 8.f), PosForPickUp, 50, 10);
        m_pScene->AddObject(*Pickup);
    }
}

void CRIPickupGenerator::SetScene(CRIGameScene& Scene) { m_pScene = &Scene; }
