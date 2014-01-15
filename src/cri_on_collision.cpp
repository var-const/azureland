#include "cri_stdafx.h"

#include "cri_on_collision.h"

#include "cri_game_objects.h"

void OnCollision( CRIEnemy& Enemy, CRIPlayer& Player )
{
    using std::min;

    //const CRI_AABB::VecT Depth = IntersectionDepth(Enemy.GetAABB(),
    //    Player.GetAABB());
    //const CRI_AABB::VecT Normal = IntersectionNormal(Enemy.GetAABB(),
    //    Player.GetAABB(), Depth);
    //const float Shift = min(Depth.x, Depth.y);
    //ShiftPos(Enemy, Normal * CRIMovable::PosT(Shift, Shift));
    const CRI_AABB::VecT Depth = IntersectionDepth(Enemy.GetAABB(),
        Player.GetAABB());
    const CRI_AABB::VecT Normal = IntersectionNormal(Enemy.GetAABB(),
        Player.GetAABB(), Depth);
    const float Shift = min(Depth.x, Depth.y) + 2.5f;
    //const float Shift = min(Depth.x, Depth.y) * 2.f;
    ShiftPos(Enemy, Normal * CRIMovable::PosT(Shift / 2.f, Shift / 2.f));
    ShiftPos(Player, ci::Vec2f(-1.f, -1.f) * Normal * CRIMovable::PosT(Shift / 2.f, Shift / 2.f));
    //Enemy.SetVelocity(CRIMovable::VelT());
}

void OnCollision( CRIEnemy& Lhs, CRIEnemy& Rhs)
{
    using std::min;

    //const CRI_AABB::VecT Depth = IntersectionDepth(Lhs.GetAABB(),
    //    Rhs.GetAABB());
    //const CRI_AABB::VecT Normal = IntersectionNormal(Lhs.GetAABB(),
    //    Rhs.GetAABB(), Depth);
    //const float Shift = min(Depth.x, Depth.y);
    //ShiftPos(Lhs, Normal * CRIMovable::PosT(Shift, Shift));
    const CRI_AABB::VecT Depth = IntersectionDepth(Lhs.GetAABB(),
        Rhs.GetAABB());
    const CRI_AABB::VecT Normal = IntersectionNormal(Lhs.GetAABB(),
        Rhs.GetAABB(), Depth);
    const float Shift = min(Depth.x, Depth.y) + 2.5f;
    //const float Shift = min(Depth.x, Depth.y) * 2.f;
    ShiftPos(Lhs, Normal * CRIMovable::PosT(Shift / 2.f, Shift / 2.f));
    ShiftPos(Rhs, ci::Vec2f(-1.f, -1.f) * Normal * CRIMovable::PosT(Shift / 2.f, Shift / 2.f));
    //Lhs.SetVelocity(CRIMovable::VelT());
    //Rhs.SetVelocity(CRIMovable::VelT());
}
