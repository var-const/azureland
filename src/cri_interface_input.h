#pragma once

class KeyEvent;
namespace cinder { template <typename T> class Vec2; }
namespace cinder { namespace app { class MouseEvent; } }

// @TODO: a better name
class CRIInterfaceInput
{
public:
    virtual ~CRIInterfaceInput();

    virtual void on_mouse_down(const cinder::Vec2<float>& Pos,
        cinder::app::MouseEvent Event);
    virtual void on_mouse_up(const cinder::Vec2<float>& Pos,
        cinder::app::MouseEvent Event);
    virtual void on_mouse_move(const cinder::Vec2<float>& Pos,
        cinder::app::MouseEvent Event);
    virtual void on_mouse_drag(const cinder::Vec2<float>& Pos,
        cinder::app::MouseEvent Event);
    virtual void on_mouse_wheel(float Increment,
        cinder::app::MouseEvent Event);
    virtual void on_key_down(KeyCode keycode, KeyEvent Event);
    virtual void on_key_up(KeyCode keycode, KeyEvent Event);
};
