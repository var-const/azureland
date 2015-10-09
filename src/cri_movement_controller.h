#pragma once

namespace cinder { template <typename T> class Vec2; }

class CRIMovable;

class CRIMovementController
{
public:
    void SetSpeed(float Speed);

    cinder::Vec2<float> GetVector(int Key) const;

    void Deccelerate(CRIMovable& Object);

    void OnKeyDown(int Key, CRIMovable& Object);
    void OnKeyUp(int key);

private:
    // @TODO: need a flag for both movement keys
    bool m_IsMoveKeyPressed{};
    float m_Speed{};
};
