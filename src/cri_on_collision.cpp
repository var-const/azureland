#include "cri_stdafx.h"

#include "cri_on_collision.h"

#include "cri_game_objects.h"

void OnCollision( CRIEnemy& Enemy, CRIPlayer& Player )
{
    using std::min;

    const CRI_AABB::VecT Depth = IntersectionDepth(Enemy.GetAABB(),
        Player.GetAABB());
    const CRI_AABB::VecT Normal = IntersectionNormal(Enemy.GetAABB(),
        Player.GetAABB(), Depth);
    const float Shift = min(Depth.x, Depth.y) + 2.5f;
    ShiftPos(Enemy, Normal * Shift * 0.9f);
    ShiftPos(Player, Normal * Shift * -0.1f);
    //ShiftPos(Enemy, Normal * Shift * 0.75f);
    //ShiftPos(Player, Normal * Shift * -0.25f);
    //ShiftPos(Enemy, Normal * CRIMovable::PosT(Shift / 2.f, Shift / 2.f));
    //ShiftPos(Player, ci::Vec2f(-1.f, -1.f) * Normal * CRIMovable::PosT(Shift / 2.f, Shift / 2.f));
}

void OnCollision( CRIEnemy& Lhs, CRIEnemy& Rhs)
{
    using std::min;

    const CRI_AABB::VecT Depth = IntersectionDepth(Lhs.GetAABB(),
        Rhs.GetAABB());
    const CRI_AABB::VecT Normal = IntersectionNormal(Lhs.GetAABB(),
        Rhs.GetAABB(), Depth);
    const float Shift = min(Depth.x, Depth.y) + 2.5f;
    ShiftPos(Lhs, Normal * CRIMovable::PosT(Shift / 2.f, Shift / 2.f));
    ShiftPos(Rhs, ci::Vec2f(-1.f, -1.f) * Normal * CRIMovable::PosT(Shift / 2.f, Shift / 2.f));
    Lhs.Collide(Rhs);
    Rhs.Collide(Lhs);
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
