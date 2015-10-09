#pragma once

#include <cinder/Timer.h>
#include <cinder/app/AppBasic.h>

#include <memory>

class CRIGameScene;
class CRIInterfaceInput;

class CRIApp : public ci::app::AppBasic
{
public:	
    CRIApp();
    ~CRIApp();

    void prepareSettings(Settings* settings) override;
    void setup() override;

    void draw() override;
    void update() override;

    void mouseDown(ci::app::MouseEvent Event) override;
    void mouseUp(ci::app::MouseEvent Event) override;
    void mouseMove(ci::app::MouseEvent Event) override;
    void mouseDrag(ci::app::MouseEvent Event) override;
    void mouseWheel(ci::app::MouseEvent Event) override;
    void keyDown(ci::app::KeyEvent Event) override;
    void keyUp(ci::app::KeyEvent Event) override;

    void SetScene(std::unique_ptr<CRIGameScene> Scene);
    void AddInputListener(CRIInterfaceInput& Listener);
    void RemoveInputListener(CRIInterfaceInput& Listener);

private:
    typedef std::vector<CRIInterfaceInput*> InputListenersContT;

    // No need
    CRIApp(const CRIApp&);
    CRIApp& operator=(const CRIApp&);

    std::unique_ptr<CRIGameScene> m_pScene;

    InputListenersContT m_InputListeners; // Non-owning

    ci::Timer m_Timer;
};
