#pragma once

#include "cri_weapon.h"

template <typename T> class Vec2;

class CRICrossbow : public CRIWeapon
{
private:
    void DoShoot(Vec2<float> StartPos,
        Vec2<float> TargetPos); // override
};
