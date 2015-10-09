#include "stdafx.h"

#include "pickups.h"

HealthPickup::HealthPickup(const SizeT& Size, const PosT& StartPos,
    const int Amount, const int LifetimeSeconds)
  : GameObject(Size, StartPos)
  , m_Amount(Amount)
{
    m_Timer.SetExpiresFromNow(static_cast<double>(LifetimeSeconds));
    SetTextureDescriptor(TextureDescr);
}

int HealthPickup::GetAmount() const { return m_Amount; }

void HealthPickup::LogicUpdate(const float Dt)
{
    if (!IsDying() && m_Timer.IsExpired()) {
        Destroy();
    }
}

int HealthPickup::TextureDescr = -1;
