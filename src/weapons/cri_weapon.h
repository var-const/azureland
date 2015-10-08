#pragma once

#include "cri_reload.h"

template <typename T> class Vec2;

class CRIGameScene;
class CRIWeapon;

class CRIWeapon
{
public:	
    CRIWeapon();
    virtual ~CRIWeapon();

    void SetReloadTime(int Milliseconds);
    void Shoot(Vec2<float> StartPos, Vec2<float> TargetPos);
    bool IsReady() const;

    void SetScene(CRIGameScene& Scene);

protected:
    CRIGameScene& GetScene();
    const CRIGameScene& GetScene() const;

private:
    virtual void DoShoot(Vec2<float> StartPos,
        Vec2<float> TargetPos) = 0;

    CRIReload m_Reload;
    CRIGameScene* m_pScene; // Non-owning
};
