#include "stdafx.h"

#include "enemy.h"

#include "collider.h"
#include "game_scene.h"
#include "math_helpers.h"
#include "player.h"

#include <cinder/CinderMath.h>
#include <cinder/Rand.h>
#include <cinder/Rect.h>
#include <cinder/Vector.h>

#include <cmath>
#include <utility>

const int SleepAfterReadjustingPos = 3;
const int SleepAfterBlocking = 3;
const int SleepAfterRespawnAttempt = 10;
const int SleepAfterLosingTrail = 15;

Enemy::Enemy(Player& Player, const SizeT& Size, const PosT& StartPos)
  : GameObject(Size, StartPos)
  , m_pPlayer(&Player)
{
    SetMaxHealth(30);        // @FIXME hardcoded
    ForceSetHealthValue(30); // @FIXME hardcoded
    m_Reload.SetReloadTime(800);
}

void Enemy::LogicUpdate(const float Dt)
{
    if (m_IsParalyzed) {
        m_IsParalyzed = m_ParalyzedTimer.IsExpired();
        return;
    }

    UpdateAngle();

    if (m_Sleep) {
        --m_Sleep;
        return;
    }

    if (m_NeedRespawn) {
        TryRespawn();
    }

    if (m_CheckBlocked) {
        m_CheckBlocked = false;
        CheckBlocked();
        return;
    }
    if (m_Blocked) {
        CheckBlocked();
        return;
    }

    m_Sleep = SleepAfterReadjustingPos;

    BoxT player_box = m_pPlayer->GetAABB();
    player_box.m_HalfSize.x += 15.f;
    player_box.m_HalfSize.y += 15.f;
    if (TouchOrIntersect(GetAABB(), player_box)) {
        OnCaughtPlayer();
        return;
    }
    PosT Direction = m_pPlayer->GetCenterPos() - GetCenterPos();
    if (Direction.length() > m_MaxPursuitRange) {
        m_Sleep = SleepAfterLosingTrail;
        return;
    }
    Direction.normalize();
    SetVelocity(Direction * m_Speed);
}

void Enemy::OnCaughtPlayer()
{
    if (IsDying()) {
        return;
    }
    SetVelocity(VelT());

    // @TODO: refactor out as weapon
    if (m_Reload.IsReady()) {
        m_pPlayer->ModifyHealth(-20); // @FIXME hard coded
        m_Reload.OnShot();
    }
}

void Enemy::OnCollisionWithEnemy(const Enemy& Rhs)
{
    using std::find;

    const float MyDistance = m_pPlayer->GetCenterPos().distance(GetCenterPos());
    const float OtherDistance =
        m_pPlayer->GetCenterPos().distance(Rhs.GetCenterPos());
    if (MyDistance > OtherDistance) {
        if (find(m_Blockers.begin(), m_Blockers.end(), &Rhs) ==
            m_Blockers.end()) {
            m_Blockers.push_back(&Rhs);
        }
        m_CheckBlocked = true;
    }
}

void Enemy::CheckBlocked()
{
    using ci::Vec2f;
    using ci::Vec2i;
    using std::abs;

    bool Blocked = false;
    for (auto i = m_Blockers.begin(); i != m_Blockers.end(); /*omitted*/) {
        if (!Intersect(GetMovementAABB(), (*i)->GetMovementAABB())) {
            i = m_Blockers.erase(i);
            continue;
        }

        Vec2f MyDistance = m_pPlayer->GetCenterPos() - GetCenterPos();
        Vec2f OtherDistance = m_pPlayer->GetCenterPos() - (*i)->GetCenterPos();
        if (MyDistance.length() < OtherDistance.length()) {
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
        if (BlockedX || BlockedY) {
            if (!m_Blocked) {
                m_Blocked = true;
                if (Diff.x > Diff.y) {
                    NewVelocity.x = 0.f;
                    NewVelocity.y = NewVelocity.y > 0.f ? m_Speed : -m_Speed;
                }
                else {
                    NewVelocity.y = 0.f;
                    NewVelocity.x = NewVelocity.x > 0.f ? m_Speed : -m_Speed;
                }
            }
        }
        else if (m_Blocked) {
            NewVelocity = NewVelocity * -1.f;
        }

        SetVelocity(NewVelocity);

        ++i;
    }

    if (m_Blocked && !Blocked) {
        m_Blocked = false;
    }
    else {
        m_Sleep = SleepAfterBlocking;
    }
}

void Enemy::OnHealthDepleted()
{
    assert(m_pPlayer);
    m_pPlayer->AddScore(m_PointsForKilling);

    SetDying();
    m_NeedRespawn = true;
    TryRespawn();
}

void Enemy::TryRespawn()
{
    using ci::randBool;
    using ci::Rectf;
    using ci::Vec2i;
    using std::pair;

    const Collider& Collider = GetScene().GetCollider();
    Vec2i Rows, Cols;
    if (randBool()) {
        Rows = randBool() ? Vec2i(0, 1) : Vec2i(-2, -1);
        Cols = Vec2i(0, 1000);
    }
    else {
        Cols = randBool() ? Vec2i(0, 1) : Vec2i(-2, -1);
        Rows = Vec2i(0, 1000);
    }

    const pair<bool, Rectf> EmptyCell = Collider.GetEmptyCell(Rows, Cols);
    if (EmptyCell.first) {
        const Rectf& Cell = EmptyCell.second;
        assert(Cell.getWidth() > GetSize().x && Cell.getHeight() > GetSize().y);
        Respawn(Cell.getCenter());
        return;
    }

    m_Sleep = SleepAfterRespawnAttempt;
}

void Enemy::Respawn(const PosT Pos)
{
    GetScene().OnEnemyRespawn(GetCenterPos());

    m_NeedRespawn = false;

    Ressurect();

    SetCenterPos(Pos);

    SetMaxHealth(20);        // @FIXME hardcoded
    ForceSetHealthValue(20); // @FIXME hardcoded
}

void Enemy::SetParalyzed(const int Milliseconds)
{
    SetVelocity(VelT());
    m_IsParalyzed = true;
    m_ParalyzedTimer.SetExpiresFromNow(static_cast<int>(Milliseconds) / 1000.0);
}

bool Enemy::IsParalyzed() const { return m_IsParalyzed; }

void Enemy::SetSpeed(const int Speed)
{
    assert(Speed >= 0);
    m_Speed = Speed;
}

void Enemy::UpdateAngle()
{
    using ci::Vec2f;
    using ci::math;

    Vec2f Dir = m_pPlayer->GetCenterPos() - GetCenterPos();
    Dir.safeNormalize();
    const float Angle =
        math<float>::atan2(0.f, -1.f) - math<float>::atan2(Dir.x, Dir.y);
    SetAngle(Angle * 180.f / M_PI);
}
