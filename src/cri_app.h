#pragma once

#include "cri_timer.h"
#include "fps_limit.h"
#include "math_rect.h"
#include "math_vector.h"

#include <SDL2pp/Renderer.hh>
#include <SDL2pp/SDL2pp.hh>
#include <SDL2pp/SDLImage.hh>
#include <SDL2pp/SDLTTF.hh>
#include <SDL2pp/Window.hh>

#include <vector>

class CRIGameScene;
class CRIInterfaceInput;
class KeyEvent;
class MouseEvent;

class CRIApp
{
public:
    CRIApp();
    ~CRIApp();

    void draw();
    bool update();

    void on_mouse_down(MouseEvent Event);
    void on_mouse_up(MouseEvent Event);
    void on_mouse_move(MouseEvent Event);
    void on_mouse_drag(MouseEvent Event);
    void on_mouse_wheel(MouseEvent Event);
    void on_key_down(KeyEvent Event);
    void on_key_up(KeyEvent Event);

    void SetScene(CRIGameScene* pScene);
    void AddInputListener(CRIInterfaceInput& Listener);
    void RemoveInputListener(CRIInterfaceInput& Listener);

    Vec2i get_window_size() const { return Vec2i{window_.GetSize()}; }
    Vec2i get_window_pos() const { return Vec2i{window_.GetPosition()}; }

private:
    typedef std::vector<CRIInterfaceInput*> InputListenersContT;
    typedef InputListenersContT::iterator InputListenersIterT;

    // No need
    CRIApp(const CRIApp&);
    CRIApp& operator=(const CRIApp&);

    CRIGameScene* m_pScene;

    InputListenersContT m_InputListeners; // Non-owning

    Timer m_Timer;
    FPSLimit fps_limit_;

    SDL2pp::SDL library_;
    SDL2pp::Window window_;
    SDL2pp::Renderer renderer_;
    SDL2pp::SDLImage image_loading_;
    SDL2pp::SDLTTF ttf_;
};

Rectd get_window_bounds(const CRIApp& app)
{
    auto const pos = app.get_window_pos();
    return {pos, pos + app.get_window_size() / 2};
}
