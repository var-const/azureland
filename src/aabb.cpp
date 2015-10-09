#include "stdafx.h"

#include "aabb.h"

#include <cinder/Rect.h>

#include <cmath>

#ifdef PASS_BY_VALUE
AABB::AABB(const VecT Center, const VecT HalfSize)
#else
AABB::AABB(VecT const& Center, VecT const& HalfSize)
#endif
  : m_Center(Center)
  , m_HalfSize(HalfSize)
{
}

#ifdef PASS_BY_VALUE
AABB::AABB(const ci::Rectf Rect)
#else
AABB::AABB(ci::Rectf const& Rect)
#endif
  : m_Center(Rect.getCenter())
  , m_HalfSize(Rect.getSize() / 2.f)
{
}

#ifdef PASS_BY_VALUE
bool Intersect(const AABB A, const AABB B)
{
    using std::abs;
    return abs(A.m_Center.x - B.m_Center.x) < A.m_HalfSize.x + B.m_HalfSize.x &&
           abs(A.m_Center.y - B.m_Center.y) < A.m_HalfSize.y + B.m_HalfSize.y;
}

bool TouchOrIntersect(const AABB A, const AABB B)
{
    using std::abs;
    return abs(A.m_Center.x - B.m_Center.x) <=
               A.m_HalfSize.x + B.m_HalfSize.x &&
           abs(A.m_Center.y - B.m_Center.y) <= A.m_HalfSize.y + B.m_HalfSize.y;
}

bool TouchOnly(const AABB A, const AABB B)
{
    return TouchOrIntersect(A, B) && !Intersect(A, B);
}

AABB::VecT IntersectionDepth(const AABB A, const AABB B)
{
    using std::abs;
    AABB::VecT Dist = B.m_Center - A.m_Center;
    Dist.x = abs(Dist.x);
    Dist.y = abs(Dist.y);
    return A.m_HalfSize + B.m_HalfSize - Dist;
}

AABB::VecT IntersectionNormal(const AABB A, const AABB B)
{
    return IntersectionNormal(A, B, IntersectionDepth(A, B));
}

AABB::VecT IntersectionNormal(
    const AABB A, const AABB B, const AABB::VecT Depth)
{
    AABB::VecT Result;
    if (!(Depth.x < Depth.y)) {
        Result.y = (A.m_Center.y - B.m_Center.y > 0.f ? 1.f : -1.f);
    }
    if (!(Depth.y < Depth.x)) {
        Result.x = (A.m_Center.x - B.m_Center.x > 0.f ? 1.f : -1.f);
    }
    return Result;
}

bool AContainsB(const AABB A, const AABB B)
{
    using std::abs;
    typedef AABB::VecT VecT;

    const VecT CenterDist = VecT(
        abs(A.m_Center.x - B.m_Center.x), abs(A.m_Center.y - B.m_Center.y));
    const VecT MaxDist = VecT(abs(A.m_HalfSize.x - B.m_HalfSize.x),
        abs(A.m_HalfSize.y - B.m_HalfSize.y));
    return CenterDist.x <= MaxDist.x && CenterDist.y <= MaxDist.y;
}

ci::Rectf ToRect(const AABB Box)
{
    using ci::Rectf;
    return Rectf(Box.m_Center - Box.m_HalfSize, Box.m_Center + Box.m_HalfSize);
}

void SetAABB(AABB& Box, const AABB::VecT Center, const AABB::VecT HalfSize)
{
    Box.m_Center = Center;
    Box.m_HalfSize = HalfSize;
}

AABB::VecT GetLeftUpper(AABB Box) { return Box.m_Center - Box.m_HalfSize; }

AABB::VecT GetRightLower(AABB Box) { return Box.m_Center + Box.m_HalfSize; }

#else
bool Intersect(AABB const& A, AABB const& B)
{
    using std::abs;
    return abs(A.m_Center.x - B.m_Center.x) < A.m_HalfSize.x + B.m_HalfSize.x &&
           abs(A.m_Center.y - B.m_Center.y) < A.m_HalfSize.y + B.m_HalfSize.y;
}

bool TouchOrIntersect(AABB const& A, AABB const& B)
{
    using std::abs;
    return abs(A.m_Center.x - B.m_Center.x) <=
               A.m_HalfSize.x + B.m_HalfSize.x &&
           abs(A.m_Center.y - B.m_Center.y) <= A.m_HalfSize.y + B.m_HalfSize.y;
}

bool TouchOnly(AABB const& A, AABB const& B)
{
    return TouchOrIntersect(A, B) && !Intersect(A, B);
}

AABB::VecT IntersectionDepth(AABB const& A, AABB const& B)
{
    using std::abs;
    AABB::VecT Dist = B.m_Center - A.m_Center;
    Dist.x = abs(Dist.x);
    Dist.y = abs(Dist.y);
    return A.m_HalfSize + B.m_HalfSize - Dist;
}

AABB::VecT IntersectionNormal(AABB const& A, AABB const& B)
{
    return IntersectionNormal(A, B, IntersectionDepth(A, B));
}

AABB::VecT IntersectionNormal(
    AABB const& A, AABB const& B, AABB::VecT const& Depth)
{
    AABB::VecT Result;
    if (!(Depth.x < Depth.y)) {
        Result.y = (A.m_Center.y - B.m_Center.y > 0.f ? 1.f : -1.f);
    }
    if (!(Depth.y < Depth.x)) {
        Result.x = (A.m_Center.x - B.m_Center.x > 0.f ? 1.f : -1.f);
    }
    return Result;
}

bool AContainsB(AABB const& A, AABB const& B)
{
    using std::abs;
    typedef AABB::VecT VecT;

    const VecT CenterDist = VecT(
        abs(A.m_Center.x - B.m_Center.x), abs(A.m_Center.y - B.m_Center.y));
    const VecT MaxDist = VecT(abs(A.m_HalfSize.x - B.m_HalfSize.x),
        abs(A.m_HalfSize.y - B.m_HalfSize.y));
    return CenterDist.x <= MaxDist.x && CenterDist.y <= MaxDist.y;
}

ci::Rectf ToRect(AABB const& Box)
{
    using ci::Rectf;
    return Rectf(Box.m_Center - Box.m_HalfSize, Box.m_Center + Box.m_HalfSize);
}

void SetAABB(AABB& Box, AABB::VecT const& Center, AABB::VecT const& HalfSize)
{
    Box.m_Center = Center;
    Box.m_HalfSize = HalfSize;
}

AABB::VecT GetLeftUpper(AABB const& Box)
{
    return Box.m_Center - Box.m_HalfSize;
}

AABB::VecT GetRightLower(AABB const& Box)
{
    return Box.m_Center + Box.m_HalfSize;
}

#endif
