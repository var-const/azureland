#include "cri_stdafx.h"

#include "cri_enemy.h"

#include "cri_collider.h"
#include "cri_game_scene.h"
#include "cri_math.h"
#include "cri_player.h"

#include <cinder/Rand.h>
#include <cinder/Rect.h>
#include <cinder/Vector.h>

#include <utility>

const int SleepAfterReadjustingPos = 3;
const int SleepAfterBlocking = 3;
const int SleepAfterRespawnAttempt = 10;
const int SleepAfterLosingTrail = 15;
const float Speed = 100.f;

CRIEnemy::CRIEnemy( CRIPlayer& Player, const SizeT& Size, const PosT& StartPos )
: CRIGameObject(Size, StartPos)
, m_pPlayer(&Player)
, m_Sleep(0)
, m_Blocked(false)
, m_CheckBlocked(false)
, m_NeedRespawn(false)
, m_PointsForKilling(10)
, m_MaxPursuitRange(2000.f)
, m_IsParalyzed(false)
{ 
    SetMaxHealth(20); // @FIXME hardcoded
    ForceSetHealthValue(20); // @FIXME hardcoded
    m_Reload.SetReloadTime(1000);
}

void CRIEnemy::LogicUpdate(const float Dt)
{
    if (m_IsParalyzed)
    {
        m_IsParalyzed = m_ParalyzedTimer.IsExpired();
        return;
    }

    if (m_Sleep)
    {
        --m_Sleep;
        return;
    }

    if (m_NeedRespawn)
    {
        TryRespawn();
    }

    if (m_CheckBlocked)
    {
        m_CheckBlocked = false;
        CheckBlocked();
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
        OnCaughtPlayer();
        return;
    }
    PosT Direction = m_pPlayer->GetCenterPos() - GetCenterPos();
    if (Direction.length() > m_MaxPursuitRange)
    {
        m_Sleep = SleepAfterLosingTrail;
        return;
    }
    Direction.normalize();
    SetVelocity(Direction * Speed);
}

void CRIEnemy::OnCaughtPlayer()
{
    SetVelocity(VelT());

    // @TODO: refactor out as weapon
    if (m_Reload.IsReady())
    {
        m_pPlayer->ModifyHealth(-10); // @FIXME hard coded
        m_Reload.OnShot();
    }
}

void CRIEnemy::OnCollisionWithEnemy( const CRIEnemy& Rhs )
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

void CRIEnemy::OnHealthDepleted()
{
    assert(m_pPlayer);
    m_pPlayer->AddScore(m_PointsForKilling);

    SetDying();
    m_NeedRespawn = true;
    TryRespawn();
}

void CRIEnemy::TryRespawn()
{
    using ci::randBool; using ci::Rectf; using ci::Vec2i;
    using std::pair;

    const CRICollider& Collider = GetScene().GetCollider();
    Vec2i Rows, Cols;
    if (randBool())
    {
        Rows = randBool() ? Vec2i(0, 1) : Vec2i(-2, -1);
        Cols = Vec2i(0, 1000);
    }
    else
    {
        Cols = randBool() ? Vec2i(0, 1) : Vec2i(-2, -1);
        Rows = Vec2i(0, 1000);
    }

    const pair<bool, Rectf> EmptyCell = Collider.GetEmptyCell(Rows, Cols);
    if (EmptyCell.first)
    {
        const Rectf& Cell = EmptyCell.second;
        assert(Cell.getWidth() > GetSize().x && Cell.getHeight() > GetSize().y);
        Respawn(Cell.getCenter());
        return;
    }

    m_Sleep = SleepAfterRespawnAttempt;
}

void CRIEnemy::Respawn( const PosT Pos )
{
    GetScene().OnEnemyRespawn(GetCenterPos());

    m_NeedRespawn = false;

    Ressurect();

    SetCenterPos(Pos);

    SetMaxHealth(20); // @FIXME hardcoded
    ForceSetHealthValue(20); // @FIXME hardcoded
}

void CRIEnemy::SetParalyzed( const int Milliseconds )
{
    SetVelocity(VelT());
    m_IsParalyzed = true;
    m_ParalyzedTimer.SetExpiresFromNow(static_cast<int>(Milliseconds) / 1000.0);
}

bool CRIEnemy::IsParalyzed() const
{
    return m_IsParalyzed;
}
