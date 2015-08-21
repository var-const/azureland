#pragma once

#include "key_event.h"
#include "mouse_event.h"

#include <cinder/Timer.h>

#include <SDL2pp/Renderer.hh>
#include <SDL2pp/SDL2pp.hh>
#include <SDL2pp/Window.hh>

#include <vector>

class CRIGameScene;
class CRIInterfaceInput;

class CRIApp
{
public:
    CRIApp();
    ~CRIApp();

    void draw();
    bool update();

    void on_mouse_down(ci::app::MouseEvent Event);
    void on_mouse_up(ci::app::MouseEvent Event);
    void on_mouse_move(ci::app::MouseEvent Event);
    void on_mouse_drag(ci::app::MouseEvent Event);
    void on_mouse_wheel(ci::app::MouseEvent Event);
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

    ci::Timer m_Timer;

    SDL2pp::SDL library_;
    SDL2pp::Window window_;
    SDL2pp::Renderer renderer_;
};
