#include "cri_stdafx.h"

#include "cri_movement_controller.h"

#include "cri_movable.h"

#include <cinder/Vector.h>
#include <cinder/app/KeyEvent.h>

#include <cmath>

namespace
{

typedef ci::app::KeyEvent KeysT;

bool IsMoveKey(const int Key)
{
    switch (Key)
    {
    case KeysT::KEY_LEFT:
    case KeysT::KEY_a:
    case KeysT::KEY_RIGHT:
    case KeysT::KEY_d:
    case KeysT::KEY_UP:
    case KeysT::KEY_w:
    case KeysT::KEY_DOWN:
    case KeysT::KEY_s:
        return true;
    default:
        return false;
    }
}

} // unnamed

CRIMovementController::CRIMovementController()
: m_IsMoveKeyPressed(false)
, m_Speed(0.f)
{}

void CRIMovementController::SetSpeed(const float Speed)
{
    m_Speed = Speed;
}

cinder::Vec2<float> CRIMovementController::GetVector(const int Key) const
{
    CRIMovable::VelT Result;

    switch (Key)
    {
    case KeysT::KEY_LEFT:
    case KeysT::KEY_a:
        Result.x = -m_Speed;
        break;
    case KeysT::KEY_RIGHT:
    case KeysT::KEY_d:
        Result.x = m_Speed;
        break;
    case KeysT::KEY_UP:
    case KeysT::KEY_w:
        Result.y = -m_Speed;
        break;
    case KeysT::KEY_DOWN:
    case KeysT::KEY_s:
        Result.y = m_Speed;
        break;
    }

    return Result;
}

void CRIMovementController::Deccelerate(CRIMovable& Object)
{
    // Avoid jerky movements
    if (m_IsMoveKeyPressed)
    {
        return;
    }
    else
    {
        Object.SetVelocity(CRIMovable::VelT());
    }
}

void CRIMovementController::OnKeyDown(const int Key, CRIMovable& Object)
{
    using std::abs;

    if (!IsMoveKey(Key))
    {
        return;
    }
    else
    {
        m_IsMoveKeyPressed = true;
    }

    // @FIXME hacky
    const CRIMovable::VelT Vec = GetVector(Key);
    CRIMovable::VelT NewVelocity = Object.GetVelocity();
    if (abs(Vec.x) >= abs(NewVelocity.x))
    {
        NewVelocity.x = Vec.x;
    }
    if (abs(Vec.y) >= abs(NewVelocity.y))
    {
        NewVelocity.y = Vec.y;
    }
    Object.SetVelocity(NewVelocity);
}

void CRIMovementController::OnKeyUp(const int Key)
{
    if (m_IsMoveKeyPressed && IsMoveKey(Key))
    {
        m_IsMoveKeyPressed = false;
    }
}
