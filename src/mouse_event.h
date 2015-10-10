#pragma once

#include "math_vector.h"

#include <SDL/SDL_events.h>

enum class MouseButton {
    Left,
    Middle,
    Right
};

class MouseEvent {
public:
    MouseEvent(SDL_MouseMotionEvent const info) : MouseEvent({info.x, info.y},
            // @FIXME always left button
            MouseButton::Left, info.state == SDL_PRESSED) {}
    MouseEvent(SDL_MouseButtonEvent const info) : MouseEvent({info.x, info.y},
            get_mouse_button_from_index(info.button), info.state == SDL_PRESSED) {}
    MouseEvent(Vec2i const from_pos, MouseButton const from_button,
        bool const from_is_pressed) : pos_{from_pos}, button_{from_button},
            is_pressed_{from_is_pressed} {}

    Vec2i get_pos() const { return pos_; }
    bool is_left() const { return button_ == MouseButton::Left; }
    bool is_right() const { return button_ == MouseButton::Right; }
    bool is_middle() const { return button_ == MouseButton::Middle; }
    bool is_pressed() const { return is_pressed_; }
    bool is_released() const { return is_released_; }

private:
    Vec2i pos_;
    MouseButton button_{};
    bool is_pressed_{};
};
