#pragma once

#include <cinder/Timer.h>
#include <cinder/app/AppBasic.h>

#include <memory>

class GameScene;
class InterfaceInput;

class App : public ci::app::AppBasic {
  public:
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

    void SetScene(std::unique_ptr<GameScene> Scene);
    void AddInputListener(InterfaceInput& Listener);
    void RemoveInputListener(InterfaceInput& Listener);

  private:
    typedef std::vector<InterfaceInput*> InputListenersContT;

    // No need
    App(const App&);
    App& operator=(const App&);

    std::unique_ptr<GameScene> m_pScene;

    InputListenersContT m_InputListeners; // Non-owning

    ci::Timer m_Timer;
};
