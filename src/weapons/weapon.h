#pragma once

#include "reload.h"

namespace cinder {
template <typename T> class Vec2;
}

class CRIGameScene;
class CRIWeapon;

class CRIWeapon {
  public:
    virtual ~CRIWeapon() {}

    void SetReloadTime(int Milliseconds);
    void Shoot(cinder::Vec2<float> StartPos, cinder::Vec2<float> TargetPos);
    bool IsReady() const;

    void SetScene(CRIGameScene& Scene);

  protected:
    CRIGameScene& GetScene();
    const CRIGameScene& GetScene() const;

  private:
    virtual void DoShoot(
        cinder::Vec2<float> StartPos, cinder::Vec2<float> TargetPos) = 0;

    CRIReload m_Reload;
    CRIGameScene* m_pScene{};
};
