#include "stdafx.h"

#include "aabb_int.h"

#include <cinder/Rect.h>

#include <cmath>

#ifdef PASS_BY_VALUE
AABBd::AABBd(const VecT Center, const VecT HalfSize)
  : m_Center(Center)
  , m_HalfSize(HalfSize)
{
}

AABBd::AABBd(const ci::Rectf Rect)
  : m_Center(Rect.getCenter())
  , m_HalfSize(Rect.getSize() / 2 + ci::Vec2i(1, 1))
{
}

bool Intersect(const AABBd A, const AABBd B)
{
    using std::abs;
    return abs(A.m_Center.x - B.m_Center.x) < A.m_HalfSize.x + B.m_HalfSize.x &&
           abs(A.m_Center.y - B.m_Center.y) < A.m_HalfSize.y + B.m_HalfSize.y;
}

bool TouchOrIntersect(const AABBd A, const AABBd B)
{
    using std::abs;

    return abs(A.m_Center.x - B.m_Center.x) <=
               A.m_HalfSize.x + B.m_HalfSize.x &&
           abs(A.m_Center.y - B.m_Center.y) <= A.m_HalfSize.y + B.m_HalfSize.y;
}

bool TouchOnly(const AABBd A, const AABBd B)
{
    return TouchOrIntersect(A, B) && !Intersect(A, B);
}

AABBd::VecT IntersectionDepth(const AABBd A, const AABBd B)
{
    using std::abs;
    AABBd::VecT Dist = B.m_Center - A.m_Center;
    Dist.x = abs(Dist.x);
    Dist.y = abs(Dist.y);
    return A.m_HalfSize + B.m_HalfSize - Dist;
}

AABBd::VecT IntersectionNormal(const AABBd A, const AABBd B)
{
    return IntersectionNormal(A, B, IntersectionDepth(A, B));
}

AABBd::VecT IntersectionNormal(
    const AABBd A, const AABBd B, const AABBd::VecT Depth)
{
    AABBd::VecT Result;
    if (!(Depth.x < Depth.y)) {
        Result.y = (A.m_Center.y - B.m_Center.y > 0 ? 1 : -1);
    }
    if (!(Depth.y < Depth.x)) {
        Result.x = (A.m_Center.x - B.m_Center.x > 0 ? 1 : -1);
    }
    return Result;
}

bool AContainsB(const AABBd A, const AABBd B)
{
    using std::abs;
    typedef AABBd::VecT VecT;

    const VecT CenterDist = VecT(
        abs(A.m_Center.x - B.m_Center.x), abs(A.m_Center.y - B.m_Center.y));
    const VecT MaxDist = VecT(abs(A.m_HalfSize.x - B.m_HalfSize.x),
        abs(A.m_HalfSize.y - B.m_HalfSize.y));
    return CenterDist.x <= MaxDist.x && CenterDist.y <= MaxDist.y;
}

ci::Rectf ToRect(const AABBd Box)
{
    using ci::Rectf;
    return Rectf(Box.m_Center - Box.m_HalfSize, Box.m_Center + Box.m_HalfSize);
}

void SetAABB(AABBd& Box, const AABBd::VecT Center, const AABBd::VecT HalfSize)
{
    Box.m_Center = Center;
    Box.m_HalfSize = HalfSize;
}

AABBd::VecT GetLeftUpper(AABBd Box) { return Box.m_Center - Box.m_HalfSize; }

AABBd::VecT GetRightLower(AABBd Box) { return Box.m_Center + Box.m_HalfSize; }

#else
AABBd::AABBd(VecT const& Center, VecT const& HalfSize)
  : m_Center(Center)
  , m_HalfSize(HalfSize)
{
}

AABBd::AABBd(ci::Rectf const& Rect)
  : m_Center(Rect.getCenter())
  , m_HalfSize(Rect.getSize() / 2 + ci::Vec2i(1, 1))
{
}

bool Intersect(AABBd const& A, AABBd const& B)
{
    using std::abs;
    return abs(A.m_Center.x - B.m_Center.x) < A.m_HalfSize.x + B.m_HalfSize.x &&
           abs(A.m_Center.y - B.m_Center.y) < A.m_HalfSize.y + B.m_HalfSize.y;
}

bool TouchOrIntersect(AABBd const& A, AABBd const& B)
{
    using std::abs;
    return abs(A.m_Center.x - B.m_Center.x) <=
               A.m_HalfSize.x + B.m_HalfSize.x &&
           abs(A.m_Center.y - B.m_Center.y) <= A.m_HalfSize.y + B.m_HalfSize.y;
}

bool TouchOnly(AABBd const& A, AABBd const& B)
{
    return TouchOrIntersect(A, B) && !Intersect(A, B);
}

AABBd::VecT IntersectionDepth(AABBd const& A, AABBd const& B)
{
    using std::abs;
    AABBd::VecT Dist = B.m_Center - A.m_Center;
    Dist.x = abs(Dist.x);
    Dist.y = abs(Dist.y);
    return A.m_HalfSize + B.m_HalfSize - Dist;
}

AABBd::VecT IntersectionNormal(AABBd const& A, AABBd const& B)
{
    return IntersectionNormal(A, B, IntersectionDepth(A, B));
}

AABBd::VecT IntersectionNormal(
    AABBd const& A, AABBd const& B, AABBd::VecT Depth)
{
    AABBd::VecT Result;
    if (!(Depth.x < Depth.y)) {
        Result.y = (A.m_Center.y - B.m_Center.y > 0 ? 1 : -1);
    }
    if (!(Depth.y < Depth.x)) {
        Result.x = (A.m_Center.x - B.m_Center.x > 0 ? 1 : -1);
    }
    return Result;
}

bool AContainsB(AABBd const& A, AABBd const& B)
{
    using std::abs;
    typedef AABBd::VecT VecT;

    VecT CenterDist = VecT(
        abs(A.m_Center.x - B.m_Center.x), abs(A.m_Center.y - B.m_Center.y));
    VecT MaxDist = VecT(abs(A.m_HalfSize.x - B.m_HalfSize.x),
        abs(A.m_HalfSize.y - B.m_HalfSize.y));
    return CenterDist.x <= MaxDist.x && CenterDist.y <= MaxDist.y;
}

ci::Rectf ToRect(AABBd const& Box)
{
    using ci::Rectf;
    return Rectf(Box.m_Center - Box.m_HalfSize, Box.m_Center + Box.m_HalfSize);
}

void SetAABB(AABBd& Box, AABBd::VecT Center, AABBd::VecT HalfSize)
{
    Box.m_Center = Center;
    Box.m_HalfSize = HalfSize;
}

AABBd::VecT GetLeftUpper(AABBd const& Box)
{
    return Box.m_Center - Box.m_HalfSize;
}

AABBd::VecT GetRightLower(AABBd const& Box)
{
    return Box.m_Center + Box.m_HalfSize;
}
#endif
