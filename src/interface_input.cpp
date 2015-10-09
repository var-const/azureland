#include "stdafx.h"

#include "interface_input.h"

#include <cinder/Vector.h>
#include <cinder/app/KeyEvent.h>
#include <cinder/app/MouseEvent.h>

using ci::Vec2f;
using ci::app::KeyEvent;
using ci::app::MouseEvent;

void InterfaceInput::OnMouseDown(const Vec2f& Pos, const MouseEvent Event) {}

void InterfaceInput::OnMouseUp(const Vec2f& Pos, const MouseEvent Event) {}

void InterfaceInput::OnMouseMove(const Vec2f& Pos, const MouseEvent Event) {}

void InterfaceInput::OnMouseDrag(const Vec2f& Pos, const MouseEvent Event) {}

void InterfaceInput::OnMouseWheel(const float Increment, const MouseEvent Event)
{
}

void InterfaceInput::OnKeyDown(const int KeyCode, const KeyEvent Event) {}

void InterfaceInput::OnKeyUp(const int KeyCode, const KeyEvent Event) {}
