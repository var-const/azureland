#pragma once

#include "game_object.h"
#include "timer.h"

class HealthPickup : public GameObject {
  public:
    HealthPickup(const SizeT& Size, const PosT& StartPos, int Amount,
        int LifetimeSeconds);

    int GetAmount() const;

    void LogicUpdate(float Dt) override;

    static int TextureDescr;

  private:
    int m_Amount{};
    CountdownTimer m_Timer;
};
