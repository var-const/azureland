#include "cri_stdafx.h"

#include "cri_pickups.h"

CRIHealthPickup::CRIHealthPickup( const SizeT& Size, const PosT& StartPos,
    const int Amount, const int LifetimeSeconds )
: CRIGameObject(Size, StartPos)
, m_Amount(Amount)
{
    m_Timer.SetExpiresFromNow(static_cast<double>(LifetimeSeconds));
    SetTextureDescriptor(TextureDescr);
}

int CRIHealthPickup::GetAmount() const
{
    return m_Amount;
}

void CRIHealthPickup::LogicUpdate( const float Dt )
{
    if (!IsDying() && m_Timer.IsExpired())
    {
        Destroy();
    }
}

int CRIHealthPickup::TextureDescr = -1;
