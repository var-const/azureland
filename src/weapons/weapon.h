#pragma once

#include "reload.h"

namespace cinder {
template <typename T> class Vec2;
}

class GameScene;
class Weapon;

class Weapon {
  public:
    virtual ~Weapon() {}

    void SetReloadTime(int Milliseconds);
    void Shoot(cinder::Vec2<float> StartPos, cinder::Vec2<float> TargetPos);
    bool IsReady() const;

    void SetScene(GameScene& Scene);

  protected:
    GameScene& GetScene();
    const GameScene& GetScene() const;

  private:
    virtual void DoShoot(
        cinder::Vec2<float> StartPos, cinder::Vec2<float> TargetPos) = 0;

    Reload m_Reload;
    GameScene* m_pScene{};
};
