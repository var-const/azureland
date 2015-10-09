#pragma once

#include "cri_weapon.h"

namespace cinder { template <typename T> class Vec2; }

class CRIPlayer;

class CRIForcefieldEmitter : public CRIWeapon
{
public:
    CRIForcefieldEmitter();
    explicit CRIForcefieldEmitter(const CRIPlayer& Player);

    void SetPlayer(const CRIPlayer& Player);

private:
    void DoShoot(cinder::Vec2<float> StartPos,
        cinder::Vec2<float> TargetPos) override;

    const CRIPlayer* m_pPlayer;
};
