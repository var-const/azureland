#pragma once

#include "cri_timer.h"
#include "fps_limit.h"

#include <SDL2pp/Renderer.hh>
#include <SDL2pp/SDL2pp.hh>
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
};
