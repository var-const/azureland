#pragma once

#include "math_rect.h"
#include "math_vector.h"

struct CRI_AABB
{
    typedef Vec2f VecT;

    CRI_AABB();
#ifdef PASS_BY_VALUE
    explicit CRI_AABB(Rectf Rect);
    CRI_AABB(VecT Center, VecT HalfSize);
#else
    explicit CRI_AABB(Rectf const& Rect);
    CRI_AABB(VecT const& Center, VecT const& HalfSize);
#endif

    VecT m_Center;
    VecT m_HalfSize;
};

#ifdef PASS_BY_VALUE
bool Intersect(CRI_AABB A, CRI_AABB B);
bool TouchOrIntersect(CRI_AABB A, CRI_AABB B);
bool TouchOnly(CRI_AABB A, CRI_AABB B);

CRI_AABB::VecT IntersectionDepth(CRI_AABB A, CRI_AABB B);
CRI_AABB::VecT IntersectionNormal(CRI_AABB A, CRI_AABB B);
CRI_AABB::VecT IntersectionNormal(CRI_AABB A, CRI_AABB B, CRI_AABB::VecT depth);

bool AContainsB(CRI_AABB A, CRI_AABB B);

Rectf ToRect(CRI_AABB Box);
void SetAABB(CRI_AABB& Box, CRI_AABB::VecT Center, CRI_AABB::VecT HalfSize);

CRI_AABB::VecT GetLeftUpper(CRI_AABB Box);
CRI_AABB::VecT GetRightLower(CRI_AABB Box);
#else
bool Intersect(CRI_AABB const& A, CRI_AABB const& B);
bool TouchOrIntersect(CRI_AABB const& A, CRI_AABB const& B);
bool TouchOnly(CRI_AABB const& A, CRI_AABB const& B);

CRI_AABB::VecT IntersectionDepth(CRI_AABB const& A, CRI_AABB const& B);
CRI_AABB::VecT IntersectionNormal(CRI_AABB const& A, CRI_AABB const& B);
CRI_AABB::VecT IntersectionNormal(CRI_AABB const& A, CRI_AABB const& B, CRI_AABB::VecT const& depth);

bool AContainsB(CRI_AABB const& A, CRI_AABB const& B);

Rectf ToRect(CRI_AABB const& Box);
void SetAABB(CRI_AABB& Box, CRI_AABB::VecT const& Center, CRI_AABB::VecT const& HalfSize);

CRI_AABB::VecT GetLeftUpper(CRI_AABB const& Box);
CRI_AABB::VecT GetRightLower(CRI_AABB const& Box);
#endif
