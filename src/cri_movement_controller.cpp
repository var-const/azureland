#include "cri_stdafx.h"

#include "cri_movement_controller.h"

#include "cri_movable.h"
#include "key_event.h"

#include "math_vector.h"

#include <cmath>

namespace
{

bool IsMoveKey(const int Key)
{
    switch (Key)
    {
    case KeyCode::Left:
    case KeyCode::a:
    case KeyCode::Right:
    case KeyCode::d:
    case KeyCode::Up:
    case KeyCode::w:
    case KeyCode::Down:
    case KeyCode::s:
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

Vec2<float> CRIMovementController::GetVector(const int Key) const
{
    CRIMovable::VelT Result;

    switch (Key)
    {
    case KeyCode::Left:
    case KeyCode::a:
        Result.x = -m_Speed;
        break;
    case KeyCode::Right:
    case KeyCode::d:
        Result.x = m_Speed;
        break;
    case KeyCode::Up:
    case KeyCode::w:
        Result.y = -m_Speed;
        break;
    case KeyCode::Down:
    case KeyCode::s:
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

void CRIMovementController::on_key_down(const int Key, CRIMovable& Object)
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

void CRIMovementController::on_key_up(const int Key)
{
    if (m_IsMoveKeyPressed && IsMoveKey(Key))
    {
        m_IsMoveKeyPressed = false;
    }
}
