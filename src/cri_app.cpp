#include "cri_stdafx.h"

#include "cri_app.h"

#include "cri_build_game.h"
#include "cri_game_scene.h"
#include "cri_interface_input.h"

#include <cassert>

CRIApp::CRIApp()
: m_pScene(0)
{ 
}

CRIApp::~CRIApp()
{
    delete m_pScene;
}

void CRIApp::prepareSettings( Settings* settings )
{
    settings->setWindowSize(1280, 1024);
    settings->setFrameRate(60.f);
}

void CRIApp::setup()
{
    hideCursor();
    m_Timer.start();
    BuildGame(*this);
}

void CRIApp::draw()
{
    if (m_pScene)
    {
        m_pScene->Draw();
    }
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

CINDER_APP_BASIC( CRIApp, ci::app::RendererGl )
