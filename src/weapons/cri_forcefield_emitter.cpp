#include "cri_stdafx.h"

#include "cri_forcefield_emitter.h"

#include "cri_forcefield.h"
#include "../cri_game_scene.h"

#include <cinder/Vector.h>

using ci::Vec2f;

void CRIForcefieldEmitter::DoShoot( const Vec2f StartPos, const Vec2f TargetPos )
{
    typedef CRIMovable::SizeT SizeT;

    CRIForcefield* const Field = new CRIForcefield(100.f, StartPos, 300, 10,
        500.f, 0.5f);
    GetScene().AddObject(*Field);
}
