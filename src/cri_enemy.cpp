#include "cri_stdafx.h"

#include "cri_enemy.h"

#include "cri_player.h"

CRIEnemy::CRIEnemy( CRIPlayer& Player, const SizeT& Size, const PosT& StartPos )
: CRIGameObject(Size, StartPos)
, m_pPlayer(&Player)
{ 
}

void CRIEnemy::DoUpdate( const float Dt )
{
    if ( TouchOrIntersect(GetAABB(), m_pPlayer->GetAABB()) )
    {
        SetVelocity(VelT());
        return;
    }
    PosT Direction = m_pPlayer->GetCenterPos() - GetCenterPos();
    Direction.normalize();
    SetVelocity(Direction * 100.f);
}
