#include "cri_stdafx.h"

#include "cri_interface_input.h"
#include "key_event.h"

#include <cinder/Vector.h>
#include <cinder/app/MouseEvent.h>

using ci::Vec2f; using ci::app::MouseEvent;

CRIInterfaceInput::~CRIInterfaceInput()
{
}

void CRIInterfaceInput::on_mouse_down( const Vec2f& Pos, const MouseEvent Event )
{
}

void CRIInterfaceInput::on_mouse_up( const Vec2f& Pos, const MouseEvent Event )
{
}

void CRIInterfaceInput::on_mouse_move( const Vec2f& Pos, const MouseEvent Event )
{
}

void CRIInterfaceInput::on_mouse_drag( const Vec2f& Pos, const MouseEvent Event )
{
}

void CRIInterfaceInput::on_mouse_wheel( const float Increment, const MouseEvent Event )
{
}

void CRIInterfaceInput::on_key_down( const KeyCode keycode, const KeyEvent Event )
{
}

void CRIInterfaceInput::on_key_up( const KeyCode keycode, const KeyEvent Event )
{
}
