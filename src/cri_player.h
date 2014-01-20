#pragma once

#include "cri_game_object.h"
#include "cri_health_mixin.h"
#include "cri_interface_input.h"
#include "cri_movement_controller.h"

class CRIApp;
class CRICrosshair;
class CRITextObject;
class CRIWeapon;

class CRIPlayer : public CRIGameObject, public CRIHealthMixin,
    public CRIInterfaceInput
{
public:	
    CRIPlayer(const SizeT& Size, const PosT& StartPos, int Health, CRIApp& App);
    ~CRIPlayer();

    void SetSpeed(float Speed);
    void AddScore(int Amount);

    // Overrides

    void OnMouseDown(const cinder::Vec2<float>& Pos,
        cinder::app::MouseEvent Event);
    void OnMouseUp(const cinder::Vec2<float>& Pos,
        cinder::app::MouseEvent Event);
    void OnMouseMove(const cinder::Vec2<float>& Pos,
        cinder::app::MouseEvent Event);
    void OnMouseDrag(const cinder::Vec2<float>& Pos,
        cinder::app::MouseEvent Event);
    void OnMouseWheel(float Increment, cinder::app::MouseEvent Event);
    void OnKeyDown(int KeyCode, cinder::app::KeyEvent Event);
    void OnKeyUp(int KeyCode, cinder::app::KeyEvent Event);

    void LogicUpdate(float Dt); // Override
    void BeforeRemoveFromScene(); // Override

private:
    void DoUpdate(float Dt); // Override
    void OnDestroyed(); // Override
    void OnAddedToScene(); // Override

    void OnHealthModified(int NewVal, int Modifier); // Override
    void OnHealthDepleted(); // Override

    void Shoot(CRIWeapon& Weapon);
    PosT GetCrosshairPos() const;

    CRIMovementController m_MovementController;

    CRICrosshair* m_pCrosshair;
    CRIWeapon* m_pWeaponA;
    bool m_AutofireWeaponA;
    CRIWeapon* m_pWeaponB;
    bool m_AutofireWeaponB;

    CRITextObject* m_pHealthLabel;
    CRITextObject* m_pScoreLabel;
    int m_Score;

    CRIApp* m_pApp; // Non-owning
};
