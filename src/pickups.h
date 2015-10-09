#pragma once

#include "game_object.h"
#include "timer.h"

class CRIHealthPickup : public CRIGameObject {
  public:
    CRIHealthPickup(const SizeT& Size, const PosT& StartPos, int Amount,
        int LifetimeSeconds);

    int GetAmount() const;

    void LogicUpdate(float Dt) override;

    static int TextureDescr;

  private:
    int m_Amount{};
    CRICountdownTimer m_Timer;
};
