#pragma once

#include <cinder/Timer.h>
#include <cinder/app/AppBasic.h>

class CRIGameScene;

class CRIApp : public ci::app::AppBasic
{
public:	
    CRIApp();
    ~CRIApp();

    // Implemented virtual functions
    void prepareSettings(Settings* settings);
    void setup();

    void draw();
    void update();

    /* For future use
    void mouseDown(ci::app::MouseEvent event);
    void mouseUp(ci::app::MouseEvent event);
    void mouseMove(ci::app::MouseEvent event);
    void mouseDrag(ci::app::MouseEvent event);
    void mouseWheel(ci::app::MouseEvent event);
    void keyDown(ci::app::KeyEvent event);
    void keyUp(ci::app::KeyEvent event);
    */

    void SetScene(CRIGameScene* pScene);

private:
    // No need
    CRIApp(const CRIApp&);
    CRIApp& operator=(const CRIApp&);

    CRIGameScene* m_pScene;

    ci::Timer m_Timer;
};
