#pragma once

#include <cinder/Rect.h>
#include <cinder/Vector.h>

struct AABB {
    typedef ci::Vec2f VecT;

    AABB() = default;
#ifdef PASS_BY_VALUE
    explicit AABB(ci::Rectf Rect);
    AABB(VecT Center, VecT HalfSize);
#else
    explicit AABB(ci::Rectf const& Rect);
    AABB(VecT const& Center, VecT const& HalfSize);
#endif

    VecT m_Center;
    VecT m_HalfSize;
};

#ifdef PASS_BY_VALUE
bool Intersect(AABB A, AABB B);
bool TouchOrIntersect(AABB A, AABB B);
bool TouchOnly(AABB A, AABB B);

AABB::VecT IntersectionDepth(AABB A, AABB B);
AABB::VecT IntersectionNormal(AABB A, AABB B);
AABB::VecT IntersectionNormal(AABB A, AABB B, AABB::VecT depth);

bool AContainsB(AABB A, AABB B);

ci::Rectf ToRect(AABB Box);
void SetAABB(AABB& Box, AABB::VecT Center, AABB::VecT HalfSize);

AABB::VecT GetLeftUpper(AABB Box);
AABB::VecT GetRightLower(AABB Box);
#else
bool Intersect(AABB const& A, AABB const& B);
bool TouchOrIntersect(AABB const& A, AABB const& B);
bool TouchOnly(AABB const& A, AABB const& B);

AABB::VecT IntersectionDepth(AABB const& A, AABB const& B);
AABB::VecT IntersectionNormal(AABB const& A, AABB const& B);
AABB::VecT IntersectionNormal(
    AABB const& A, AABB const& B, AABB::VecT const& depth);

bool AContainsB(AABB const& A, AABB const& B);

ci::Rectf ToRect(AABB const& Box);
void SetAABB(AABB& Box, AABB::VecT const& Center, AABB::VecT const& HalfSize);

AABB::VecT GetLeftUpper(AABB const& Box);
AABB::VecT GetRightLower(AABB const& Box);
#endif
