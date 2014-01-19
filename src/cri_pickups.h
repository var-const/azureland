#pragma once

#include "cri_game_object.h"

class CRIHealthPickup : public CRIGameObject
{
public:	
    CRIHealthPickup(const SizeT& Size, const PosT& StartPos, int Amount);

    int GetAmount() const;
    
private:
    int m_Amount;
};
