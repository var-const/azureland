#pragma once

class KeyEvent;
class MouseEvent;
template <typename T> class Vec2;

// @TODO: a better name
class CRIInterfaceInput
{
public:
    virtual ~CRIInterfaceInput();

    virtual void on_mouse_down(const Vec2<float>& Pos, MouseEvent Event);
    virtual void on_mouse_up(const Vec2<float>& Pos, MouseEvent Event);
    virtual void on_mouse_move(const Vec2<float>& Pos, MouseEvent Event);
    virtual void on_mouse_drag(const Vec2<float>& Pos, MouseEvent Event);
    virtual void on_mouse_wheel(float Increment, MouseEvent Event);
    virtual void on_key_down(KeyCode keycode, KeyEvent Event);
    virtual void on_key_up(KeyCode keycode, KeyEvent Event);
};
