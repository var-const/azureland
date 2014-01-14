#pragma once

namespace cinder { template <typename T> class Vec2; }
namespace cinder { namespace app { class KeyEvent; class MouseEvent; } }

// @TODO: a better name
class CRIInterfaceInput
{
public:	
    virtual ~CRIInterfaceInput();

    virtual void OnMouseDown(const cinder::Vec2<float>& Pos,
        cinder::app::MouseEvent Event) = 0;
    virtual void OnMouseUp(const cinder::Vec2<float>& Pos,
        cinder::app::MouseEvent Event) = 0;
    virtual void OnMouseMove(const cinder::Vec2<float>& Pos,
        cinder::app::MouseEvent Event) = 0;
    virtual void OnMouseDrag(const cinder::Vec2<float>& Pos,
        cinder::app::MouseEvent Event) = 0;
    virtual void OnMouseWheel(float Increment,
        cinder::app::MouseEvent Event) = 0;
    virtual void OnKeyDown(int KeyCode, cinder::app::KeyEvent Event) = 0;
    virtual void OnKeyUp(int KeyCode, cinder::app::KeyEvent Event) = 0;
};