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
, m_Speed(0.f)
{ 
}

void CRIPlayer::SetSpeed( const float Speed )
{
    m_Speed = Speed;
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

void CRIPlayer::OnMouseWheel( float Increment, const MouseEvent Event )
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
    // @TODO
}

void CRIPlayer::OnKeyUp( const int KeyCode, const KeyEvent Event )
{
    if (IsDying())
    {
        return;
    }
    // @TODO
}

void CRIPlayer::OnDestroyed()
{
    // @TODO
}
