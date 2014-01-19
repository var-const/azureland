#pragma once

#include "cri_game_object.h"
#include "cri_interface_input.h"
#include "cri_movement_controller.h"

class CRICrosshair;

class CRIPlayer : public CRIGameObject, public CRIInterfaceInput
{
public:	
    CRIPlayer(const SizeT& Size, const PosT& StartPos);

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

    CRIMovementController m_MovementController;
    CRICrosshair* m_pCrosshair;
};
