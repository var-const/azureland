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
    using ci::Vec2f; using ci::Vec2i;
    using std::abs;

    bool Blocked = false;
    for (BlockersIterT i = m_Blockers.begin(); i != m_Blockers.end();)
    {
        if (!Intersect(GetMovementAABB(), (*i)->GetMovementAABB()))
        {
            i = m_Blockers.erase(i);
            continue;
        }

        Vec2f MyDistance = m_pPlayer->GetCenterPos() - GetCenterPos();
        Vec2f OtherDistance = m_pPlayer->GetCenterPos() - (*i)->GetCenterPos();
        if (MyDistance.length() < OtherDistance.length())
        {
            ++i;
            continue;
        }

        Blocked = true;

        MyDistance.x = abs(MyDistance.x);
        MyDistance.y = abs(MyDistance.y);
        OtherDistance.x = abs(OtherDistance.x);
        OtherDistance.y = abs(OtherDistance.y);
        const Vec2f Diff = MyDistance - OtherDistance;
        const Vec2i Normal = IntersectionNormal(GetAABB(), (*i)->GetAABB());
        VelT NewVelocity = GetVelocity();

        const bool BlockedX = Normal.x != 0 && Diff.x > Diff.y;
        const bool BlockedY = Normal.y != 0 && Diff.y > Diff.x;
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
        // ≈сли он ближе, но не по оси столкновени€, и мы обходим, надо мен€ть направление
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
