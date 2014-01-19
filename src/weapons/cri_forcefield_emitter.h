#pragma once

#include "cri_weapon.h"

namespace cinder { template <typename T> class Vec2; }

class CRIForcefieldEmitter : public CRIWeapon
{
private:
    void DoShoot(cinder::Vec2<float> StartPos,
        cinder::Vec2<float> TargetPos); // override
};
