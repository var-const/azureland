#pragma once

namespace cinder {
template <typename T> class Vec2;
}
namespace cinder {
namespace app {
class KeyEvent;
class MouseEvent;
}
}

// @TODO: a better name
class InterfaceInput {
  public:
    virtual ~InterfaceInput() {}

    virtual void OnMouseDown(
        const cinder::Vec2<float>& Pos, cinder::app::MouseEvent Event);
    virtual void OnMouseUp(
        const cinder::Vec2<float>& Pos, cinder::app::MouseEvent Event);
    virtual void OnMouseMove(
        const cinder::Vec2<float>& Pos, cinder::app::MouseEvent Event);
    virtual void OnMouseDrag(
        const cinder::Vec2<float>& Pos, cinder::app::MouseEvent Event);
    virtual void OnMouseWheel(float Increment, cinder::app::MouseEvent Event);
    virtual void OnKeyDown(int KeyCode, cinder::app::KeyEvent Event);
    virtual void OnKeyUp(int KeyCode, cinder::app::KeyEvent Event);
};
