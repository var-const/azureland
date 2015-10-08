#pragma once

template <typename T> class Vec2;

class CRIMovable;

class CRIMovementController
{
public:
    CRIMovementController();

    void SetSpeed(float Speed);

    Vec2<float> GetVector(int Key) const;

    void Deccelerate(CRIMovable& Object);

    void on_key_down(int Key, CRIMovable& Object);
    void on_key_up(int key);

private:
    // @TODO: need a flag for both movement keys
    bool m_IsMoveKeyPressed;
    float m_Speed;
};
