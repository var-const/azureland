#include "cri_stdafx.h"

#include "cri_on_collision.h"

#include "cri_game_objects.h"

void OnCollision( CRIEnemy& Enemy, CRIPlayer& Player )
{
    using std::min;

    if (Enemy.IsDying() || Player.IsDying())
    {
        return;
    }

    const CRI_AABB::VecT Depth = IntersectionDepth(Enemy.GetAABB(),
        Player.GetAABB());
    const CRI_AABB::VecT Normal = IntersectionNormal(Enemy.GetAABB(),
        Player.GetAABB(), Depth);
    const float Shift = min(Depth.x, Depth.y) + 2.5f;
    ShiftPos(Enemy, Normal * Shift * 0.9f);
    ShiftPos(Player, Normal * Shift * -0.1f);
}

void OnCollision( CRIEnemy& Lhs, CRIEnemy& Rhs)
{
    using std::min;

    if (Lhs.IsDying() || Rhs.IsDying())
    {
        return;
    }

    const CRI_AABB::VecT Depth = IntersectionDepth(Lhs.GetAABB(),
        Rhs.GetAABB());
    const CRI_AABB::VecT Normal = IntersectionNormal(Lhs.GetAABB(),
        Rhs.GetAABB(), Depth);
    const float Shift = min(Depth.x, Depth.y) + 2.5f;
    ShiftPos(Lhs, Normal * Shift / 2.f);
    ShiftPos(Rhs, Normal * Shift * -1.f / 2.f);

    Lhs.OnCollisionWithEnemy(Rhs);
    Rhs.OnCollisionWithEnemy(Lhs);
}

void OnCollision( CRIEnemy& Lhs, CRIObstacle& Rhs )
{
    using std::min;

    const CRI_AABB::VecT Depth = IntersectionDepth(Lhs.GetAABB(),
        Rhs.GetAABB());
    const CRI_AABB::VecT Normal = IntersectionNormal(Lhs.GetAABB(),
        Rhs.GetAABB(), Depth);
    const float Shift = min(Depth.x, Depth.y) + 2.5f;
    ShiftPos(Lhs, Normal * Shift);
}

void OnCollision( CRIPlayer& Lhs, CRIObstacle& Rhs )
{
    using std::min;

    const CRI_AABB::VecT Depth = IntersectionDepth(Lhs.GetAABB(),
        Rhs.GetAABB());
    const CRI_AABB::VecT Normal = IntersectionNormal(Lhs.GetAABB(),
        Rhs.GetAABB(), Depth);
    const float Shift = min(Depth.x, Depth.y) + 2.5f;
    ShiftPos(Lhs, Normal * Shift);
}

void OnCollision( CRIEnemy& Lhs, CRIProjectile& Rhs )
{
    if (Lhs.IsDying() || Rhs.IsDying())
    {
        return;
    }

    Lhs.ModifyHealth(Rhs.GetDamage() * -1);
    Rhs.Destroy();
}

void OnCollision( CRIObstacle& Lhs, CRIProjectile& Rhs )
{
    if (Rhs.IsDying())
    {
        return;
    }

    Rhs.Destroy();
}

void OnCollision( CRIPlayer& Lhs, CRIHealthPickup& Rhs )
{
    if (Lhs.IsDying() || Rhs.IsDying())
    {
        return;
    }
    if (HasFullHealth(Lhs))
    {
        return;
    }

    Lhs.ModifyHealth(Rhs.GetAmount());
    Rhs.Destroy();
}

void OnCollision( CRIEnemy& Lhs, CRIForcefield& Rhs )
{
    if (Lhs.IsDying() || Rhs.IsDying())
    {
        return;
    }
    if (Rhs.Affect(Lhs))
    {
        Lhs.ModifyHealth(Rhs.GetDamage() * -1);
        Lhs.SetParalyzed(1500);
        Lhs.SetVelocity(Rhs.GetPushVector(Lhs.GetCenterPos()));
    }
}

void OnCollision( CRIEnemy& Lhs, CRIHealthPickup& Rhs )
{
    if (Lhs.IsDying() || Rhs.IsDying())
    {
        return;
    }
    Rhs.Destroy();
}
