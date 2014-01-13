#pragma once

#include <cinder/Timer.h>
#include <cinder/app/AppBasic.h>

class CRIGameScene;
class CRIInterfaceInput;

class CRIApp : public ci::app::AppBasic
{
public:	
    CRIApp();
    ~CRIApp();

    // Overriden virtuals

    void prepareSettings(Settings* settings);
    void setup();

    void draw();
    void update();

    void mouseDown(ci::app::MouseEvent Event);
    void mouseUp(ci::app::MouseEvent Event);
    void mouseMove(ci::app::MouseEvent Event);
    void mouseDrag(ci::app::MouseEvent Event);
    void mouseWheel(ci::app::MouseEvent Event);
    void keyDown(ci::app::KeyEvent Event);
    void keyUp(ci::app::KeyEvent Event);

    // Own functions

    void SetScene(CRIGameScene* pScene);
    void AddInputListener(CRIInterfaceInput& Listener);

private:
    typedef std::vector<CRIInterfaceInput*> InputListenersContT;
    typedef InputListenersContT::iterator InputListenersIterT;

    // No need
    CRIApp(const CRIApp&);
    CRIApp& operator=(const CRIApp&);

    CRIGameScene* m_pScene;

    InputListenersContT m_InputListeners; // Non-owning

    ci::Timer m_Timer;
};
