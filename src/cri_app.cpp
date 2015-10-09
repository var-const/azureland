#include "cri_stdafx.h"

#include "cri_app.h"

#include "cri_build_game.h"
#include "cri_game_scene.h"
#include "cri_interface_input.h"

#include <cassert>
#include <cstdlib>
#include <iosfwd>

#ifndef NO_VSYNC
#define NO_VSYNC
#endif

void CRIApp::prepareSettings(Settings* const settings)
{
    settings->setWindowSize(1280, 1024);
    settings->setFrameRate(60.f);
}

void CRIApp::setup()
{
    using std::srand;
    using std::time;

    srand(time(nullptr));
    hideCursor();
    setFpsSampleInterval(1.f);

    m_Timer.start();
    BuildGame(*this);
}

void CRIApp::draw()
{
    using namespace ci;

#ifdef NO_VSYNC
    gl::disableVerticalSync();
#endif
    gl::enableAlphaBlending();
    if (m_pScene) {
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
    if (m_pScene) {
        m_pScene->Update(static_cast<float>(m_Timer.getSeconds()));
        m_Timer.start();
    }
}

void CRIApp::SetScene(std::unique_ptr<CRIGameScene> pScene)
{
    m_pScene = std::move(pScene);
}

void CRIApp::mouseDown(const ci::app::MouseEvent Event)
{
    for (auto&& listener : m_InputListeners) {
        listener->OnMouseDown(Event.getPos(), Event);
    }
}

void CRIApp::mouseUp(const ci::app::MouseEvent Event)
{
    for (auto&& listener : m_InputListeners) {
        listener->OnMouseUp(Event.getPos(), Event);
    }
}

void CRIApp::mouseMove(const ci::app::MouseEvent Event)
{
    for (auto&& listener : m_InputListeners) {
        listener->OnMouseMove(Event.getPos(), Event);
    }
}

void CRIApp::mouseDrag(const ci::app::MouseEvent Event)
{
    for (auto&& listener : m_InputListeners) {
        listener->OnMouseDrag(Event.getPos(), Event);
    }
}

void CRIApp::mouseWheel(const ci::app::MouseEvent Event)
{
    for (auto&& listener : m_InputListeners) {
        listener->OnMouseWheel(Event.getWheelIncrement(), Event);
    }
}

void CRIApp::keyDown(const ci::app::KeyEvent Event)
{
    for (auto&& listener : m_InputListeners) {
        listener->OnKeyDown(Event.getCode(), Event);
    }
}

void CRIApp::keyUp(const ci::app::KeyEvent Event)
{
    for (auto&& listener : m_InputListeners) {
        listener->OnKeyUp(Event.getCode(), Event);
    }
}

void CRIApp::AddInputListener(CRIInterfaceInput& Listener)
{
    assert(std::find(m_InputListeners.begin(), m_InputListeners.end(),
               &Listener) == m_InputListeners.end());
    m_InputListeners.push_back(&Listener);
}

void CRIApp::RemoveInputListener(CRIInterfaceInput& Listener)
{
    using std::find;

    const auto Found =
        find(m_InputListeners.begin(), m_InputListeners.end(), &Listener);
    if (Found != m_InputListeners.end()) {
        m_InputListeners.erase(Found);
    }
    else {
        assert(0);
    }
}

CINDER_APP_BASIC(CRIApp, ci::app::RendererGl)
