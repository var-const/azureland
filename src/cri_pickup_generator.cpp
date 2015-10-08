#include "cri_stdafx.h"

#include "cri_pickup_generator.h"

#include "cri_game_scene.h"
#include "cri_pickups.h"

#include "rand.h"
#include "math_vector.h"



CRIPickupGenerator::CRIPickupGenerator()
: m_pScene(NULL)
{
}

void CRIPickupGenerator::Invoke( const Vec2f PosForPickUp )
{
    typedef CRIMovable::SizeT SizeT;

    assert(m_pScene);

    if (rand_bool())
    {
        CRIHealthPickup* const Pickup = new CRIHealthPickup(SizeT(8.f, 8.f),
            PosForPickUp, 50, 10); 
        m_pScene->AddObject(*Pickup);
    }
}

void CRIPickupGenerator::SetScene( CRIGameScene& Scene )
{
    m_pScene = &Scene;
}
