#include "cri_stdafx.h"

#include "cri_player.h"

#include <cinder/Vector.h>
#include <cinder/app/KeyEvent.h>
#include <cinder/app/MouseEvent.h>

using ci::Vec2f;
using ci::app::KeyEvent;
using ci::app::MouseEvent;

CRIPlayer::CRIPlayer( const SizeT& Size, const PosT& StartPos )
: CRIGameObject(Size, StartPos)
{ 
}

void CRIPlayer::SetSpeed( const float Speed )
{
    m_MovementController.SetSpeed(Speed);
}

void CRIPlayer::DoUpdate( const float Dt )
{
    m_MovementController.Deccelerate(*this);
}

void CRIPlayer::OnMouseDown( const Vec2f& Pos, const MouseEvent Event )
{
    if (IsDying())
    {
        return;
    }
    // @TODO
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
    // @TODO
}

void CRIPlayer::OnMouseDrag( const Vec2f& Pos, const MouseEvent Event )
{
    if (IsDying())
    {
        return;
    }
    // @TODO
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
