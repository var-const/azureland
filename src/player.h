#pragma once

#include "cri_game_object.h"
#include "cri_health_mixin.h"
#include "cri_interface_input.h"
#include "cri_movement_controller.h"

#include <cinder/gl/Texture.h>

#include <memory>

class CRIApp;
class CRICrosshair;
class CRITextObject;
class CRIWeapon;

class CRIPlayer : public CRIGameObject,
                  public CRIHealthMixin,
                  public CRIInterfaceInput {
  public:
    CRIPlayer(const SizeT& Size, const PosT& StartPos, int Health, CRIApp& App);

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

    void Shoot(CRIWeapon& Weapon);
    PosT GetCrosshairPos() const;
    void UpdateAngle();

    void Paralyze(int Frames);

    CRIMovementController m_MovementController;

    CRICrosshair* m_pCrosshair{};
    std::unique_ptr<CRIWeapon> m_pWeaponA;
    bool m_AutofireWeaponA{};
    std::unique_ptr<CRIWeapon> m_pWeaponB;
    bool m_AutofireWeaponB{};

    CRITextObject* m_pHealthLabel{};
    CRITextObject* m_pScoreLabel{};
    int m_Score{};

    CRIApp* m_pApp{};

    int m_ParalyzedCounter{};
};
