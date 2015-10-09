#pragma once

namespace cinder {
template <typename T> class Vec2;
}

class GameScene;

class PickupGenerator {
  public:
    void Invoke(cinder::Vec2<float> PosForPickUp);

    void SetScene(GameScene& Scene);

  private:
    GameScene* m_pScene{};
};
