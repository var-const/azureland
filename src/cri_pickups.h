#pragma once

#include "cri_game_object.h"
#include "cri_timer.h"

class CRIHealthPickup : public CRIGameObject
{
public:	
    CRIHealthPickup(const SizeT& Size, const PosT& StartPos, int Amount,
        int LifetimeSeconds);

    int GetAmount() const;
    
    void LogicUpdate(float Dt); // override

    static int TextureDescr;

private:
    int m_Amount;
    CRICountdownTimer m_Timer;
};
