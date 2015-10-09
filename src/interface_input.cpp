#include "cri_stdafx.h"

#include "cri_interface_input.h"

#include <cinder/Vector.h>
#include <cinder/app/KeyEvent.h>
#include <cinder/app/MouseEvent.h>

using ci::Vec2f;
using ci::app::KeyEvent;
using ci::app::MouseEvent;

void CRIInterfaceInput::OnMouseDown(const Vec2f& Pos, const MouseEvent Event) {}

void CRIInterfaceInput::OnMouseUp(const Vec2f& Pos, const MouseEvent Event) {}

void CRIInterfaceInput::OnMouseMove(const Vec2f& Pos, const MouseEvent Event) {}

void CRIInterfaceInput::OnMouseDrag(const Vec2f& Pos, const MouseEvent Event) {}

void CRIInterfaceInput::OnMouseWheel(
    const float Increment, const MouseEvent Event)
{
}

void CRIInterfaceInput::OnKeyDown(const int KeyCode, const KeyEvent Event) {}

void CRIInterfaceInput::OnKeyUp(const int KeyCode, const KeyEvent Event) {}
