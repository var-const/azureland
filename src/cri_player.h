#pragma once

#include "cri_game_object.h"
#include "cri_health_mixin.h"
#include "cri_interface_input.h"
#include "cri_movement_controller.h"

class CRICrosshair;
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

    void LogicUpdate(); // Override

private:
    void DoUpdate(float Dt); // Override
    void OnDestroyed(); // Override
    void OnAddedToScene(); // Override

    void Die(); // Override

    PosT GetCrosshairPos() const;

    CRIMovementController m_MovementController;

    CRICrosshair* m_pCrosshair;
    CRIWeapon* m_pWeaponA;
    CRIWeapon* m_pWeaponB;
};
