#pragma once

#include "cri_game_object.h"

class CRIProjectile : public CRIGameObject
{
public:	
    CRIProjectile(const SizeT& Size, const PosT& Pos, const VelT& Velocity,
        int Damage);

    int GetDamage() const;

    static int TextureDescr;

private:
    int m_Damage{};
};
