#include "cri_stdafx.h"

#include "cri_pickups.h"

CRIHealthPickup::CRIHealthPickup( const SizeT& Size, const PosT& StartPos,
    const int Amount )
: CRIGameObject(Size, StartPos)
, m_Amount(Amount)
{
}

int CRIHealthPickup::GetAmount() const
{
    return m_Amount;
}
