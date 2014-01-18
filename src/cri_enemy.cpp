#include "cri_stdafx.h"

#include "cri_enemy.h"

#include "cri_math.h"
#include "cri_player.h"

#include <cinder/Vector.h>

const int SleepAfterReadjustingPos = 3;
const int SleepAfterBlocking = 3;
const float Speed = 100.f;

CRIEnemy::CRIEnemy( CRIPlayer& Player, const SizeT& Size, const PosT& StartPos )
: CRIGameObject(Size, StartPos)
, m_pPlayer(&Player)
, m_Sleep(0)
, m_Blocked(false)
, m_CheckBlocked(false)
{ 
}

void CRIEnemy::LogicUpdate()
{
    if (m_Sleep)
    {
        --m_Sleep;
        return;
    }

    if (m_CheckBlocked)
    {
        CheckBlocked();
        m_CheckBlocked = false;
        return;
    }
    if (m_Blocked)
    {
        CheckBlocked();
        return;
    }

    m_Sleep = SleepAfterReadjustingPos;

    BoxT player_box = m_pPlayer->GetAABB();
    player_box.m_HalfSize.x += 15.f;
    player_box.m_HalfSize.y += 15.f;
    if (TouchOrIntersect(GetAABB(), player_box))
    {
        SetVelocity(VelT());
        return;
    }
    PosT Direction = m_pPlayer->GetCenterPos() - GetCenterPos();
    Direction.normalize();
    SetVelocity(Direction * Speed);
}

void CRIEnemy::Collide( const CRIEnemy& Rhs )
{
    using std::find;

    const float MyDistance = m_pPlayer->GetCenterPos().distance(GetCenterPos());
    const float OtherDistance = m_pPlayer->GetCenterPos().distance(
        Rhs.GetCenterPos());
    if (MyDistance > OtherDistance)
    {
        if (find(m_Blockers.begin(), m_Blockers.end(), &Rhs) == m_Blockers.end())
        {
            m_Blockers.push_back(&Rhs);
        }
        m_CheckBlocked = true;
    }
}

void CRIEnemy::CheckBlocked()
{
    using ci::Vec2f;
    using std::abs;

    bool Blocked = false;
    for (BlockersIterT i = m_Blockers.begin(); i != m_Blockers.end();)
    {
        if (!Intersect(GetMovementAABB(), (*i)->GetMovementAABB()))
        {
            i = m_Blockers.erase(i);
            continue;
        }

        const float MyDistance = m_pPlayer->GetCenterPos().distance(GetCenterPos());
        const float OtherDistance = m_pPlayer->GetCenterPos().distance((*i)->GetCenterPos());
        if (MyDistance < OtherDistance)
        {
            ++i;
            continue;
        }

        Blocked = true;

        const auto my_dist_x = abs(m_pPlayer->GetCenterPos().x - GetCenterPos().x);
        const auto other_dist_x = abs(m_pPlayer->GetCenterPos().x - (*i)->GetCenterPos().x);
        const auto my_dist_y = abs(m_pPlayer->GetCenterPos().y - GetCenterPos().y);
        const auto other_dist_y = abs(m_pPlayer->GetCenterPos().y - (*i)->GetCenterPos().y);
        const Vec2f Diff = Vec2f(my_dist_x - other_dist_x , my_dist_y - other_dist_y);
        const auto Normal = IntersectionNormal(GetAABB(), (*i)->GetAABB());
        VelT NewVelocity = GetVelocity();
        // Если он ближе к цели по оси столкновения, и мы не обходим, надо обходить
        const bool BlockedX = !IsFpointEq(Normal.x, 0.f) && Diff.x > Diff.y;
        const bool BlockedY = !IsFpointEq(Normal.y, 0.f) && Diff.y > Diff.x;
        if (BlockedX || BlockedY)
        {
            if (!m_Blocked)
            {
                m_Blocked = true;
                if (Diff.x > Diff.y)
                {
                    NewVelocity.x = 0.f;
                    NewVelocity.y = NewVelocity.y > 0.f ? Speed : -Speed;
                }
                else
                {
                    NewVelocity.y = 0.f;
                    NewVelocity.x = NewVelocity.x > 0.f ? Speed : -Speed;
                }
            }
        }
        // Если он ближе, но не по оси столкновения, и мы обходим, надо менять направление
        else if (m_Blocked)
        {
            NewVelocity = NewVelocity * -1.f;
        }

        SetVelocity(NewVelocity);

        ++i;
    }

    if (m_Blocked && !Blocked)
    {
        m_Blocked = false;
    }
    else
    {
        m_Sleep = SleepAfterBlocking;
    }
}
