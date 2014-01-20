#include "cri_stdafx.h"

#include "cri_forcefield_emitter.h"

#include "cri_forcefield.h"
#include "../cri_game_scene.h"

#include <cinder/Vector.h>

using ci::Vec2f;

CRIForcefieldEmitter::CRIForcefieldEmitter()
: m_pPlayer(NULL)
{
}

CRIForcefieldEmitter::CRIForcefieldEmitter( const CRIPlayer& Player )
: m_pPlayer(&Player)
{
}

void CRIForcefieldEmitter::SetPlayer( const CRIPlayer& Player )
{
    m_pPlayer = &Player;
}

void CRIForcefieldEmitter::DoShoot( const Vec2f StartPos, const Vec2f TargetPos )
{
    typedef CRIMovable::SizeT SizeT;

    assert(m_pPlayer);
    CRIForcefield* const Field = new CRIForcefield(200.f, StartPos, 300, 10,
        500.f, *m_pPlayer);
    GetScene().AddObject(*Field);
}
