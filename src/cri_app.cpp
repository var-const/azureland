#include "cri_stdafx.h"

#include "cri_app.h"

#include "cri_build_game.h"
#include "cri_game_scene.h"
#include "cri_interface_input.h"

#include <cassert>
#include <cstdlib>
#include <iosfwd>

CRIApp::CRIApp()
: m_pScene(NULL)
{ 
}

CRIApp::~CRIApp()
{
    delete m_pScene;
    m_pScene = NULL;
}

void CRIApp::prepareSettings( Settings* const settings )
{
    settings->setWindowSize(1280, 1024);
    settings->setFrameRate(60.f);
}

void CRIApp::setup()
{
    using std::srand; using std::time;

    srand(time(NULL));
    hideCursor();
    setFpsSampleInterval(1.f);

    m_Timer.start();
    BuildGame(*this);
}

void CRIApp::draw()
{
    using namespace ci;

    if (m_pScene)
    {
        gl::clear();
        m_pScene->Draw();
    }

    using std::stringstream;
    static stringstream s;
    s.str("");
    s << getAverageFps();
    gl::drawString(s.str(), Vec2f());
}

void CRIApp::update()
{
    if (m_pScene)
    {
        m_pScene->Update( static_cast<float>(m_Timer.getSeconds()) );
        m_Timer.start();
    }
}

void CRIApp::SetScene( CRIGameScene* const pScene )
{
    delete m_pScene;
    m_pScene = pScene;
}

void CRIApp::mouseDown( const ci::app::MouseEvent Event )
{
    for (InputListenersIterT i = m_InputListeners.begin();
        i != m_InputListeners.end(); ++i)
    {
        (*i)->OnMouseDown(Event.getPos(), Event);
    }
}

void CRIApp::mouseUp( const ci::app::MouseEvent Event )
{
    for (InputListenersIterT i = m_InputListeners.begin();
        i != m_InputListeners.end(); ++i)
    {
        (*i)->OnMouseUp(Event.getPos(), Event);
    }
}

void CRIApp::mouseMove( const ci::app::MouseEvent Event )
{
    for (InputListenersIterT i = m_InputListeners.begin();
        i != m_InputListeners.end(); ++i)
    {
        (*i)->OnMouseMove(Event.getPos(), Event);
    }
}

void CRIApp::mouseDrag( const ci::app::MouseEvent Event )
{
    for (InputListenersIterT i = m_InputListeners.begin();
        i != m_InputListeners.end(); ++i)
    {
        (*i)->OnMouseDrag(Event.getPos(), Event);
    }
}

void CRIApp::mouseWheel( const ci::app::MouseEvent Event )
{
    for (InputListenersIterT i = m_InputListeners.begin();
        i != m_InputListeners.end(); ++i)
    {
        (*i)->OnMouseWheel(Event.getWheelIncrement(), Event);
    }
}

void CRIApp::keyDown( const ci::app::KeyEvent Event )
{
    for (InputListenersIterT i = m_InputListeners.begin();
        i != m_InputListeners.end(); ++i)
    {
        (*i)->OnKeyDown(Event.getCode(), Event);
    }
}

void CRIApp::keyUp( const ci::app::KeyEvent Event )
{
    for (InputListenersIterT i = m_InputListeners.begin();
        i != m_InputListeners.end(); ++i)
    {
        (*i)->OnKeyUp(Event.getCode(), Event);
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

CINDER_APP_BASIC( CRIApp, ci::app::RendererGl )
