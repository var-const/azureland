#pragma once

#include "game_object.h"
#include "health_mixin.h"
#include "interface_input.h"
#include "movement_controller.h"

#include <cinder/gl/Texture.h>

#include <memory>

class App;
class Crosshair;
class TextObject;
class Weapon;

class Player : public GameObject, public HealthMixin, public InterfaceInput {
  public:
    Player(const SizeT& Size, const PosT& StartPos, int Health, App& App);

    void SetSpeed(float Speed);
    void AddScore(int Amount);

    void OnMouseDown(
        const cinder::Vec2<float>& Pos, cinder::app::MouseEvent Event) override;
    void OnMouseUp(
        const cinder::Vec2<float>& Pos, cinder::app::MouseEvent Event) override;
    void OnMouseMove(
        const cinder::Vec2<float>& Pos, cinder::app::MouseEvent Event) override;
    void OnMouseDrag(
        const cinder::Vec2<float>& Pos, cinder::app::MouseEvent Event) override;
    void OnMouseWheel(float Increment, cinder::app::MouseEvent Event) override;
    void OnKeyDown(int KeyCode, cinder::app::KeyEvent Event) override;
    void OnKeyUp(int KeyCode, cinder::app::KeyEvent Event) override;

    void LogicUpdate(float Dt) override;
    void BeforeRemoveFromScene() override;

  private:
    void DoDraw() override;
    void DoUpdate(float Dt) override;
    void OnDestroyed() override;
    void OnAddedToScene() override;

    void OnHealthModified(int NewVal, int Modifier) override;
    void OnHealthDepleted() override;

    void Shoot(Weapon& Weapon);
    PosT GetCrosshairPos() const;
    void UpdateAngle();

    void Paralyze(int Frames);

    MovementController m_MovementController;

    Crosshair* m_pCrosshair{};
    std::unique_ptr<Weapon> m_pWeaponA;
    bool m_AutofireWeaponA{};
    std::unique_ptr<Weapon> m_pWeaponB;
    bool m_AutofireWeaponB{};

    TextObject* m_pHealthLabel{};
    TextObject* m_pScoreLabel{};
    int m_Score{};

    App* m_pApp{};

    int m_ParalyzedCounter{};
};
