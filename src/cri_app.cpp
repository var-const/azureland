#include "cri_stdafx.h"

#include "cri_app.h"

#include "cri_build_game.h"
#include "cri_game_scene.h"
#include "cri_interface_input.h"
// @TODO: implement
#include "render_string.h"

#include <SDL/SDL_events.h>
#include <SDL/SDL_mouse.h>

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iosfwd>

#ifndef NO_VSYNC
#define NO_VSYNC
#endif

CRIApp::CRIApp()
: m_pScene(NULL)
, library_{SDL_INIT_VIDEO}
, window_{"Azureland 0.2", SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED, 1280, 1024, SDL_WINDOW_RESIZABLE}
, renderer_{window_, -1, SDL_RENDERER_ACCELERATED}
, fps_limit_{60}
{
    SDL_ShowCursor(SDL_DISABLE);
    m_Timer.start();
    // @TODO: show FPS

    BuildGame(*this);
}

CRIApp::~CRIApp()
{
    delete m_pScene;
    m_pScene = NULL;
}

void CRIApp::draw()
{
#ifdef NO_VSYNC
    gl::disableVerticalSync();
#endif
    renderer_.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
    if (m_pScene)
    {
        renderer_.Clear();
        m_pScene->Draw();
    }

    using std::stringstream;
    static stringstream s;
    s.str("");
    s << getAverageFps();
    render_string(s.str());
}

bool CRIApp::update()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_KEYDOWN:
                on_key_down();
                break;
            case SDL_KEYUP:
                break;
            case SDL_QUIT:
                return false;
            default:
                break;
        }
    }

    auto const dt = static_cast<float>(m_Timer.get_seconds());
    if (m_pScene)
    {
        m_pScene->Update(dt);
    }
    fps_limit_.enforce(m_Timer);
    m_Timer.start();

    return true;
}

void CRIApp::SetScene( CRIGameScene* const pScene )
{
    delete m_pScene;
    m_pScene = pScene;
}

void CRIApp::on_mouse_down( const MouseEvent Event )
{
    for (InputListenersIterT i = m_InputListeners.begin();
        i != m_InputListeners.end(); ++i)
    {
        (*i)->on_mouse_down(Event.get_pos(), Event);
    }
}

void CRIApp::on_mouse_up( const MouseEvent Event )
{
    for (InputListenersIterT i = m_InputListeners.begin();
        i != m_InputListeners.end(); ++i)
    {
        (*i)->on_mouse_up(Event.get_pos(), Event);
    }
}

void CRIApp::on_mouse_move( const MouseEvent Event )
{
    for (InputListenersIterT i = m_InputListeners.begin();
        i != m_InputListeners.end(); ++i)
    {
        (*i)->on_mouse_move(Event.get_pos(), Event);
    }
}

void CRIApp::on_mouse_drag( const MouseEvent Event )
{
    for (InputListenersIterT i = m_InputListeners.begin();
        i != m_InputListeners.end(); ++i)
    {
        (*i)->on_mouse_drag(Event.get_pos(), Event);
    }
}

void CRIApp::on_mouse_wheel( const MouseEvent Event )
{
    for (InputListenersIterT i = m_InputListeners.begin();
        i != m_InputListeners.end(); ++i)
    {
        (*i)->on_mouse_wheel(Event.getWheelIncrement(), Event);
    }
}

void CRIApp::on_key_down( const KeyEvent Event )
{
    for (InputListenersIterT i = m_InputListeners.begin();
        i != m_InputListeners.end(); ++i)
    {
        (*i)->on_key_down(Event.get_code(), Event);
    }
}

void CRIApp::on_key_up( const KeyEvent Event )
{
    for (InputListenersIterT i = m_InputListeners.begin();
        i != m_InputListeners.end(); ++i)
    {
        (*i)->on_key_up(Event.get_code(), Event);
    }
}

void CRIApp::AddInputListener( CRIInterfaceInput& Listener )
{
    assert(std::find(m_InputListeners.begin(), m_InputListeners.end(),
        &Listener) == m_InputListeners.end());
    m_InputListeners.push_back(&Listener);
}

void CRIApp::RemoveInputListener( CRIInterfaceInput& Listener )
{
    using std::find;

    const InputListenersIterT Found = find(m_InputListeners.begin(),
        m_InputListeners.end(), &Listener);
    if (Found != m_InputListeners.end())
    {
        m_InputListeners.erase(Found);
    }
    else
    {
        assert(0);
    }
}
