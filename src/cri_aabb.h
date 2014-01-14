#pragma once

#include <cinder/Rect.h>
#include <cinder/Vector.h>

struct CRI_AABB
{
    typedef ci::Vec2f VecT;

    CRI_AABB();
    explicit CRI_AABB(ci::Rectf Rect);
    CRI_AABB(VecT Center, VecT HalfSize);

    VecT m_Center;
    VecT m_HalfSize;
};

bool Intersect(CRI_AABB A, CRI_AABB B);
bool TouchOrIntersect(CRI_AABB A, CRI_AABB B);

CRI_AABB::VecT IntersectionDepth(CRI_AABB A, CRI_AABB B);
CRI_AABB::VecT IntersectionNormal(CRI_AABB A, CRI_AABB B);
CRI_AABB::VecT IntersectionNormal(CRI_AABB A, CRI_AABB B, CRI_AABB::VecT depth);

bool AContainsB(CRI_AABB A, CRI_AABB B);

ci::Rectf ToRect(CRI_AABB Box);
void SetAABB(CRI_AABB& Box, CRI_AABB::VecT Center, CRI_AABB::VecT HalfSize);

CRI_AABB::VecT GetLeftUpper(CRI_AABB Box);
CRI_AABB::VecT GetRightLower(CRI_AABB Box);