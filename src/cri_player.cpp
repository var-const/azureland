#include "cri_stdafx.h"

#include "cri_player.h"

#include "cri_crosshair.h"
#include "cri_game_scene.h"

#include <cinder/Vector.h>
#include <cinder/app/KeyEvent.h>
#include <cinder/app/MouseEvent.h>


#include "weapons/cri_crossbow.h"

using ci::Vec2f;
using ci::app::KeyEvent;
using ci::app::MouseEvent;

CRIPlayer::CRIPlayer( const SizeT& Size, const PosT& StartPos )
: CRIGameObject(Size, StartPos)
, m_pCrosshair( new CRICrosshair(SizeT(10.f, 10.f), PosT()) )
, m_pWeaponA(new CRICrossbow())
, m_pWeaponB(NULL)
{ 
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
    m_pWeaponA->SetScene(GetScene());
    //m_pWeaponB->SetScene(GetScene());
}

void CRIPlayer::SetSpeed( const float Speed )
{
    m_MovementController.SetSpeed(Speed);
}

void CRIPlayer::DoUpdate( const float Dt )
{
    m_MovementController.Deccelerate(*this);
}

void CRIPlayer::LogicUpdate()
{
    GetScene().MoveCamera(GetCenterPos());
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
        m_pWeaponA->Shoot(GetCenterPos(), GetCrosshairPos());
    }
    else if (Event.isRight())
    {
        assert(m_pWeaponA);
        m_pWeaponB->Shoot(GetCenterPos(), GetCrosshairPos());
    }
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
    // @TODO
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

void CRIPlayer::Die()
{
    Destroy();
}
