#include "stdafx.h"

#include "on_collision.h"

#include "game_objects.h"

void OnCollision(Enemy& Enemy, Player& Player)
{
    using std::min;

    if (Enemy.IsDying() || Player.IsDying()) {
        return;
    }

    const AABB::VecT Depth =
        IntersectionDepth(Enemy.GetAABB(), Player.GetAABB());
    const AABB::VecT Normal =
        IntersectionNormal(Enemy.GetAABB(), Player.GetAABB(), Depth);
    const float Shift = min(Depth.x, Depth.y) + 2.5f;
    ShiftPos(Enemy, Normal * Shift * 0.9f);
    ShiftPos(Player, Normal * Shift * -0.1f);
}

void OnCollision(Enemy& Lhs, Enemy& Rhs)
{
    using std::min;

    if (Lhs.IsDying() || Rhs.IsDying()) {
        return;
    }

    const AABB::VecT Depth = IntersectionDepth(Lhs.GetAABB(), Rhs.GetAABB());
    const AABB::VecT Normal =
        IntersectionNormal(Lhs.GetAABB(), Rhs.GetAABB(), Depth);
    const float Shift = min(Depth.x, Depth.y) + 2.5f;
    ShiftPos(Lhs, Normal * Shift / 2.f);
    ShiftPos(Rhs, Normal * Shift * -1.f / 2.f);

    Lhs.OnCollisionWithEnemy(Rhs);
    Rhs.OnCollisionWithEnemy(Lhs);
}

void OnCollision(Enemy& Lhs, Obstacle& Rhs)
{
    using std::min;

    const AABB::VecT Depth = IntersectionDepth(Lhs.GetAABB(), Rhs.GetAABB());
    const AABB::VecT Normal =
        IntersectionNormal(Lhs.GetAABB(), Rhs.GetAABB(), Depth);
    const float Shift = min(Depth.x, Depth.y) + 2.5f;
    ShiftPos(Lhs, Normal * Shift);
}

void OnCollision(Player& Lhs, Obstacle& Rhs)
{
    using std::min;

    const AABB::VecT Depth = IntersectionDepth(Lhs.GetAABB(), Rhs.GetAABB());
    const AABB::VecT Normal =
        IntersectionNormal(Lhs.GetAABB(), Rhs.GetAABB(), Depth);
    const float Shift = min(Depth.x, Depth.y) + 2.5f;
    ShiftPos(Lhs, Normal * Shift);
}

void OnCollision(Enemy& Lhs, Projectile& Rhs)
{
    if (Lhs.IsDying() || Rhs.IsDying()) {
        return;
    }

    Lhs.ModifyHealth(Rhs.GetDamage() * -1);
    Rhs.Destroy();
}

void OnCollision(Obstacle& Lhs, Projectile& Rhs)
{
    if (Rhs.IsDying()) {
        return;
    }

    Rhs.Destroy();
}

void OnCollision(Player& Lhs, HealthPickup& Rhs)
{
    if (Lhs.IsDying() || Rhs.IsDying()) {
        return;
    }
    if (!HasFullHealth(Lhs)) {
        Lhs.ModifyHealth(Rhs.GetAmount());
        return;
    }

    Rhs.Destroy();
}

void OnCollision(Enemy& Lhs, Forcefield& Rhs)
{
    if (Lhs.IsDying() || Rhs.IsDying()) {
        return;
    }
    if (Rhs.Affect(Lhs)) {
        Lhs.ModifyHealth(Rhs.GetDamage() * -1);
        Lhs.SetParalyzed(1500);
        Lhs.SetVelocity(Rhs.GetPushVector(Lhs.GetCenterPos()));
    }
}

void OnCollision(Enemy& Lhs, HealthPickup& Rhs)
{
    if (Lhs.IsDying() || Rhs.IsDying()) {
        return;
    }
    Rhs.Destroy();
}
