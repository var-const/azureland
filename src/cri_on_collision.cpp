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
    const float Shift = min(Depth.x, Depth.y);
    ShiftPos(Enemy, Normal * CRIMovable::PosT(Shift, Shift));
}

void OnCollision( CRIEnemy& Lhs, CRIEnemy& Rhs)
{
    // @TODO
}
