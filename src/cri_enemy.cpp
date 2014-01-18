#include "cri_stdafx.h"

#include "cri_enemy.h"

#include "cri_player.h"
#include "cri_math.h"
#include "cinder\Vector.h"

CRIEnemy::CRIEnemy( CRIPlayer& Player, const SizeT& Size, const PosT& StartPos )
: CRIGameObject(Size, StartPos)
, m_pPlayer(&Player)
, m_Sleep()
, m_RecalcSleep()
, m_Blocked()
{ 
}

void CRIEnemy::DoUpdate( const float Dt )
{
    if (m_Sleep)
    {
        --m_Sleep;
        return;
    }
    if (m_RecalcSleep)
    {
        --m_RecalcSleep;
        return;
    }
    //else
    //{
    //    for (auto i = m_Collide.begin(); i != m_Collide.end();)
    //    {
    //        if (Intersect(GetAABB(), (*i)->GetAABB()))
    //        {
    //            //m_Sleep = 1;
    //            return;
    //        }
    //    }
    //}
    //m_Collide.clear();

    if (!m_Blocked)
    {
        auto player_box = m_pPlayer->GetAABB();
        player_box.m_HalfSize.x += 15.f;
        player_box.m_HalfSize.y += 15.f;
        if ( TouchOrIntersect(GetAABB(), player_box) )
        //if ( TouchOrIntersect(GetAABB(), m_pPlayer->GetAABB()) )
        {
            SetVelocity(VelT());
            return;
        }
        //PosT Direction = m_pPlayer->GetCenterPos() + ci::Vec2f(0.f, -35.f) - GetCenterPos();
        PosT Direction = m_pPlayer->GetCenterPos() - GetCenterPos();
        Direction.normalize();
        SetVelocity(Direction * 100.f);
        m_RecalcSleep = 3;
    }
    else
    {
        CheckBlocked();
    }
}

void CRIEnemy::Collide( const CRIEnemy& Rhs )
{
    const float MyDistance = m_pPlayer->GetCenterPos().distance(GetCenterPos());
    const float OtherDistance = m_pPlayer->GetCenterPos().distance(Rhs.GetCenterPos());
    if (MyDistance > OtherDistance)
    {
        //SetVelocity(VelT());
        //SetVelocity(GetVelocity() * 0.9f);
        //m_Sleep = 3;
        m_Collide.insert(&Rhs);
        CheckBlocked();
    }
}

void CRIEnemy::CheckBlocked()
{
    bool blocked = false;
    for (auto i = m_Collide.begin(); i != m_Collide.end();)
    {
        if (!Intersect(GetMovementAABB(), (*i)->GetMovementAABB()))
        {
            i = m_Collide.erase(i);
            continue;
        }
        const float MyDistance = m_pPlayer->GetCenterPos().distance(GetCenterPos());
        const float OtherDistance = m_pPlayer->GetCenterPos().distance((*i)->GetCenterPos());
        if (MyDistance < OtherDistance)
        {
            ++i;
            continue;
        }

        blocked = true;

        using ci::Vec2f;
        const auto my_dist_x = std::abs(m_pPlayer->GetCenterPos().x - GetCenterPos().x);
        const auto other_dist_x = std::abs(m_pPlayer->GetCenterPos().x - (*i)->GetCenterPos().x);
        const auto my_dist_y = std::abs(m_pPlayer->GetCenterPos().y - GetCenterPos().y);
        const auto other_dist_y = std::abs(m_pPlayer->GetCenterPos().y - (*i)->GetCenterPos().y);
        const Vec2f diff = Vec2f(my_dist_x - other_dist_x , my_dist_y - other_dist_y);
        const auto normal = IntersectionNormal(GetAABB(), (*i)->GetAABB());
        auto v = GetVelocity();
        // Если он ближе к цели по оси столкновения, и мы не обходим, надо обходить
        const auto blocked_x = !IsFpointEq(normal.x, 0.f) && diff.x > diff.y;
        const auto blocked_y = !IsFpointEq(normal.y, 0.f) && diff.y > diff.x;
        if (blocked_x || blocked_y)
        {
            if (!m_Blocked)
            {
                m_Blocked = true;
                if (diff.x > diff.y)
                {
                    v.x = 0.f;
                    v.y = v.y > 0.f ? 100.f : -100.f;
                    //v.y = (*i)->GetCenterPos().y > GetCenterPos().y ? 100.f : -100.f;
                }
                else
                {
                    v.y = 0.f;
                    //v.x = (*i)->GetCenterPos().x > GetCenterPos().x ? 100.f : -100.f;
                    v.x = v.x > 0.f ? 100.f : -100.f;
                }
            }
        }
        // Если он ближе, но не по оси столкновения, и мы обходим, надо менять направление
        else if (m_Blocked)
        {
            v = v * -1.f;
        }
        // Если он ближе, но не по оси столкновения, и мы не обходим, ничего
        else
        {

        }
        SetVelocity(v);

        ++i;
    }
    if (m_Blocked && !blocked)
    {
        m_Blocked = false;
    }
    else
    {
        m_RecalcSleep = 3;
    }

        /*
        const auto normal = IntersectionNormal(GetAABB(), (*i)->GetAABB());
        if (!IsFpointEq(normal.x, 0.f) && !IsFpointEq(normal.y, 0.f))
        {
            SetVelocity(VelT());
            ++i;
            continue;
        }
        if (!IsFpointEq(normal.x, 0.f))
        {
            v.y = 100.f;
            //v.y = v.y > 0.f ? 100.f : -100.f;
            v.x = 0.f;
        }
        if (!IsFpointEq(normal.y, 0.f))
        {
            v.x = 100.f;
            //v.x = v.x > 0.f ? 100.f : -100.f;
            //v.x += v.y;
            v.y = 0.f;
        }
        SetVelocity(v);
        ++i;
        */
    //m_RecalcSleep = 3;
}
