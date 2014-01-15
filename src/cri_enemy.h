#pragma once

#include "cri_game_object.h"

class CRIPlayer;

class CRIEnemy : public CRIGameObject
{
public:	
    CRIEnemy(CRIPlayer& Player, const SizeT& Size, const PosT& StartPos);
    
private:
    void DoUpdate(float Dt); // override

    CRIPlayer* m_pPlayer;
};
