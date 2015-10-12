#include "stdafx.h"

#include "player.h"

#include "app.h"
#include "crosshair.h"
#include "game_scene.h"
#include "text_object.h"
#include "weapons/crossbow.h"
#include "weapons/forcefield_emitter.h"

#include <cinder/Font.h>
#include <cinder/Vector.h>
#include <cinder/app/KeyEvent.h>
#include <cinder/app/MouseEvent.h>

#include <cinder/CinderMath.h>

#include <cmath>

using ci::Font;
using ci::Vec2f;
using ci::app::KeyEvent;
using ci::app::MouseEvent;
using ci::gl::Texture;

Player::Player(
    const SizeT& Size, const PosT& StartPos, const int Health, App& App)
  : GameObject(Size, StartPos)
  , m_pWeaponA(new Crossbow())
  , m_pWeaponB(new ForcefieldEmitter(*this))
  , m_pApp(&App)
{
    using ci::Font;

    assert(Health > 0);

    m_pWeaponA->SetReloadTime(200);  // @FIXME hard coded
    m_pWeaponB->SetReloadTime(1500); // @FIXME hard coded

    SetMaxHealth(Health);
    ForceSetHealthValue(Health);
}

void Player::OnAddedToScene()
{
    assert(m_pWeaponA);
    assert(m_pWeaponB);

    m_pWeaponA->SetScene(GetScene());
    m_pWeaponB->SetScene(GetScene());

	auto crosshair = std::make_unique<Crosshair>(SizeT(10.f, 10.f), PosT());
	m_pCrosshair = crosshair.get();
	GetScene().AddGUIObject(std::move(crosshair));
	auto health_label = std::make_unique<TextObject>(PosT(100.f, 50.f));
	m_pHealthLabel = health_label.get();
	GetScene().AddGUIObject(std::move(health_label));
	auto score_label = std::make_unique<TextObject>(PosT(1100.f, 50.f));
	m_pScoreLabel = score_label.get();
	GetScene().AddGUIObject(std::move(score_label));

    m_pHealthLabel->SetFont(Font("Verdana", 32));
    OnHealthModified(GetCurHealthValue(), 0);
    m_pScoreLabel->SetFont(Font("Verdana", 32));
    AddScore(0);
}

void Player::SetSpeed(const float Speed)
{
    m_MovementController.SetSpeed(Speed);
}

void Player::DoUpdate(const float Dt)
{
    if (m_ParalyzedCounter) {
        --m_ParalyzedCounter;
        SetVelocity(VelT());
        return;
    }
    m_MovementController.Deccelerate(*this);
}

void Player::LogicUpdate(const float Dt)
{
    UpdateAngle();

    GetScene().MoveCamera(GetCenterPos());

    if (m_AutofireWeaponA) {
        assert(m_pWeaponA);
        Shoot(*m_pWeaponA);
    }
    if (m_AutofireWeaponB) {
        assert(m_pWeaponB);
        if (m_pWeaponB->IsReady()) {
            Paralyze(20);
        }
        Shoot(*m_pWeaponB);
    }
}

void Player::OnMouseDown(const Vec2f& Pos, const MouseEvent Event)
{
    if (IsDying()) {
        return;
    }

    if (Event.isLeft()) {
        assert(m_pWeaponA);
        Shoot(*m_pWeaponA);
        m_AutofireWeaponA = true;
    }
    else if (Event.isRight()) {
        assert(m_pWeaponB);
        if (m_pWeaponB->IsReady()) {
            Paralyze(20);
        }
        Shoot(*m_pWeaponB);
        m_AutofireWeaponB = true;
    }
}

void Player::Shoot(Weapon& Weapon)
{
    Weapon.Shoot(GetCenterPos(), GetCrosshairPos());
}

Movable::PosT Player::GetCrosshairPos() const
{
    assert(m_pCrosshair);
    return GetScene().ToGamePos(m_pCrosshair->GetCenterPos());
}

void Player::OnMouseUp(const Vec2f& Pos, const MouseEvent Event)
{
    if (IsDying()) {
        return;
    }

    if (Event.isLeft()) {
        m_AutofireWeaponA = false;
    }
    else if (Event.isRight()) {
        m_AutofireWeaponB = false;
    }
}

void Player::OnMouseMove(const Vec2f& Pos, const MouseEvent Event)
{
    if (IsDying()) {
        return;
    }

    assert(m_pCrosshair);
    m_pCrosshair->SetCenterPos(Pos);
}

void Player::OnMouseDrag(const Vec2f& Pos, const MouseEvent Event)
{
    if (IsDying()) {
        return;
    }

    OnMouseMove(Pos, Event);
}

void Player::OnMouseWheel(const float Increment, const MouseEvent Event)
{
    if (IsDying()) {
        return;
    }
    // @TODO
}

void Player::OnKeyDown(const int KeyCode, const KeyEvent Event)
{
    if (IsDying()) {
        return;
    }
    m_MovementController.OnKeyDown(KeyCode, *this);
    if (m_ParalyzedCounter) {
        SetVelocity(VelT());
    }
}

void Player::OnKeyUp(const int KeyCode, const KeyEvent Event)
{
    if (IsDying()) {
        return;
    }
    m_MovementController.OnKeyUp(KeyCode);
}

void Player::OnHealthDepleted() { Destroy(); }

void Player::OnHealthModified(const int NewVal, const int Modifier)
{
    using std::stringstream;

    assert(m_pHealthLabel);

    static stringstream stream;
    stream.str("");
    stream << "Health: " << NewVal;
    m_pHealthLabel->SetText(stream.str());
}

void Player::AddScore(const int Amount)
{
    using std::stringstream;

    assert(Amount >= 0);
    assert(m_pHealthLabel);

    m_Score += Amount;

    static stringstream stream;
    stream.str("");
    stream << "Score: " << m_Score;
    m_pScoreLabel->SetText(stream.str());
}

void Player::OnDestroyed()
{
    m_pApp->RemoveInputListener(*this);
    GetScene().EndGame(m_Score);
}

void Player::UpdateAngle()
{
    using ci::math;
    using ci::Vec2f;

    Vec2f Dir = GetCrosshairPos() - GetCenterPos();
    Dir.safeNormalize();
    const float Angle =
        math<float>::atan2(0.f, -1.f) - math<float>::atan2(Dir.x, Dir.y);
    SetAngle(Angle * 180.f / M_PI);
}

void Player::DoDraw()
{
    //::Draw(*this, GetAngle(), m_Texture);
}

void Player::Paralyze(const int Frames)
{
    m_ParalyzedCounter = Frames;
    SetVelocity(VelT());
}
