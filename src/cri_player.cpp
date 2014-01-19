#include "cri_stdafx.h"

#include "cri_player.h"

#include "cri_crosshair.h"
#include "cri_game_scene.h"
#include "cri_text_object.h"
#include "weapons/cri_crossbow.h"
#include "weapons/cri_forcefield_emitter.h"

#include <cinder/Font.h>
#include <cinder/Vector.h>
#include <cinder/app/KeyEvent.h>
#include <cinder/app/MouseEvent.h>

using ci::Vec2f;
using ci::app::KeyEvent;
using ci::app::MouseEvent;

CRIPlayer::CRIPlayer( const SizeT& Size, const PosT& StartPos )
: CRIGameObject(Size, StartPos)
, m_pCrosshair( new CRICrosshair(SizeT(10.f, 10.f), PosT()) )
, m_pWeaponA(new CRICrossbow())
, m_pWeaponB(new CRIForcefieldEmitter(*this))
, m_AutofireWeaponA(false)
, m_AutofireWeaponB(false)
, m_pHealthLabel( new CRITextObject(PosT(100.f, 50.f)) )
{ 
    using ci::Font;

    m_pWeaponA->SetReloadTime(200); // @FIXME hard coded
    m_pWeaponB->SetReloadTime(1000); // @FIXME hard coded

    SetMaxHealth(10000); // @FIXME hardcoded
    ForceSetHealthValue(10000); // @FIXME hardcoded

    m_pHealthLabel->SetFont(Font("Verdana", 32));
    OnHealthModified(GetCurHealthValue(), 0);
}

CRIPlayer::~CRIPlayer()
{
    delete m_pWeaponA;
    m_pWeaponA = NULL;
    delete m_pWeaponB;
    m_pWeaponB = NULL;
}

void CRIPlayer::OnAddedToScene()
{
    assert(m_pWeaponA);
    assert(m_pWeaponB);

    GetScene().AddGUIObject(*m_pCrosshair);
    GetScene().AddGUIObject(*m_pHealthLabel);
    m_pWeaponA->SetScene(GetScene());
    m_pWeaponB->SetScene(GetScene());
}

void CRIPlayer::SetSpeed( const float Speed )
{
    m_MovementController.SetSpeed(Speed);
}

void CRIPlayer::DoUpdate( const float Dt )
{
    m_MovementController.Deccelerate(*this);
}

void CRIPlayer::LogicUpdate(const float Dt)
{
    GetScene().MoveCamera(GetCenterPos());

    if (m_AutofireWeaponA)
    {
        assert(m_pWeaponA);
        Shoot(*m_pWeaponA);
    }
    if (m_AutofireWeaponB)
    {
        assert(m_pWeaponB);
        Shoot(*m_pWeaponB);
    }
}

void CRIPlayer::OnMouseDown( const Vec2f& Pos, const MouseEvent Event )
{
    if (IsDying())
    {
        return;
    }

    if (Event.isLeft())
    {
        assert(m_pWeaponA);
        Shoot(*m_pWeaponA);
        m_AutofireWeaponA = true;
    }
    else if (Event.isRight())
    {
        assert(m_pWeaponB);
        Shoot(*m_pWeaponB);
        m_AutofireWeaponB = true;
    }
}

void CRIPlayer::Shoot(CRIWeapon& Weapon)
{
    Weapon.Shoot(GetCenterPos(), GetCrosshairPos());
}

CRIMovable::PosT CRIPlayer::GetCrosshairPos() const
{
    assert(m_pCrosshair);
    return GetScene().ToGamePos(m_pCrosshair->GetCenterPos());
}

void CRIPlayer::OnMouseUp( const Vec2f& Pos, const MouseEvent Event )
{
    if (IsDying())
    {
        return;
    }

    if (Event.isLeft())
    {
        m_AutofireWeaponA = false;
    }
    else if (Event.isRight())
    {
        m_AutofireWeaponB = false;
    }
}

void CRIPlayer::OnMouseMove( const Vec2f& Pos, const MouseEvent Event )
{
    if (IsDying())
    {
        return;
    }

    assert(m_pCrosshair);
    m_pCrosshair->SetCenterPos(Pos);
}

void CRIPlayer::OnMouseDrag( const Vec2f& Pos, const MouseEvent Event )
{
    if (IsDying())
    {
        return;
    }

    OnMouseMove(Pos, Event);
}

void CRIPlayer::OnMouseWheel( const float Increment, const MouseEvent Event )
{
    if (IsDying())
    {
        return;
    }
    // @TODO
}

void CRIPlayer::OnKeyDown( const int KeyCode, const KeyEvent Event )
{
    if (IsDying())
    {
        return;
    }
    m_MovementController.OnKeyDown(KeyCode, *this);
}

void CRIPlayer::OnKeyUp( const int KeyCode, const KeyEvent Event )
{
    if (IsDying())
    {
        return;
    }
    m_MovementController.OnKeyUp(KeyCode);
}

void CRIPlayer::OnDestroyed()
{
    // @TODO
}

void CRIPlayer::OnHealthDepleted()
{
    Destroy();
}

void CRIPlayer::OnHealthModified( const int NewVal, const int Modifier )
{
    using std::stringstream;

    assert(m_pHealthLabel);

    static stringstream stream;
    stream.str("");
    stream << "Health: " << NewVal;
    m_pHealthLabel->SetText(stream.str());
}
