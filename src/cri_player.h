#pragma once

#include "cri_game_object.h"
#include "cri_health_mixin.h"
#include "cri_interface_input.h"
#include "cri_movement_controller.h"

class CRICrosshair;
class CRITextObject;
class CRIWeapon;

class CRIPlayer : public CRIGameObject, public CRIHealthMixin,
    public CRIInterfaceInput
{
public:	
    CRIPlayer(const SizeT& Size, const PosT& StartPos);
    ~CRIPlayer();

    void SetSpeed(float Speed);

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
};
