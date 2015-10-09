#pragma once

namespace cinder {
template <typename T> class Vec2;
}

class Movable;

class MovementController {
  public:
    void SetSpeed(float Speed);

    cinder::Vec2<float> GetVector(int Key) const;

    void Deccelerate(Movable& Object);

    void OnKeyDown(int Key, Movable& Object);
    void OnKeyUp(int key);

  private:
    // @TODO: need a flag for both movement keys
    bool m_IsMoveKeyPressed{};
    float m_Speed{};
};
