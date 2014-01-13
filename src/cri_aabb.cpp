#include "cri_stdafx.h"

#include "cri_aabb.h"

#include <cinder/Rect.h>

#include <cmath>

CRI_AABB::CRI_AABB()
{ 
}

CRI_AABB::CRI_AABB( const VecT Center, const VecT HalfSize )
: m_Center(Center)
, m_HalfSize(HalfSize)
{
}

CRI_AABB::CRI_AABB( const ci::Rectf Rect )
: m_Center(Rect.getCenter())
, m_HalfSize(Rect.getSize() / 2.f)
{
}

bool Intersect( const CRI_AABB A, const CRI_AABB B )
{
    using std::abs;
    return abs(A.m_Center.x - B.m_Center.x) < A.m_HalfSize.x + B.m_HalfSize.x &&
        abs(A.m_Center.y - B.m_Center.y) < A.m_HalfSize.y + B.m_HalfSize.y;
}

bool TouchOrIntersect( const CRI_AABB A, const CRI_AABB B )
{
    using std::abs;
    return abs(A.m_Center.x - B.m_Center.x) <=
        A.m_HalfSize.x + B.m_HalfSize.x &&
        abs(A.m_Center.y - B.m_Center.y) <=
        A.m_HalfSize.y + B.m_HalfSize.y;
}

CRI_AABB::VecT IntersectionDepth( const CRI_AABB A, const CRI_AABB B )
{
    using std::abs;
    CRI_AABB::VecT Dist = B.m_Center - A.m_Center;
    Dist.x = abs(Dist.x);
    Dist.y = abs(Dist.y);
    return A.m_HalfSize + B.m_HalfSize - Dist;
}

CRI_AABB::VecT IntersectionNormal( const CRI_AABB A, const CRI_AABB B )
{
    return IntersectionNormal(A, B, IntersectionDepth(A, B));
}

CRI_AABB::VecT IntersectionNormal( const CRI_AABB A, const CRI_AABB B,
    const CRI_AABB::VecT Depth )
{
    CRI_AABB::VecT Result;
    if (!(Depth.x < Depth.y)) {
        Result.y = (A.m_Center.y - B.m_Center.y > 0.f ? 1.f : -1.f);
    }
    if (!(Depth.y < Depth.x)) {
        Result.x = (A.m_Center.x - B.m_Center.x > 0.f ? 1.f : -1.f);
    }
    return Result;
}

bool AContainsB( const CRI_AABB A, const CRI_AABB B )
{
    using std::abs;
    typedef CRI_AABB::VecT VecT;

    const VecT CenterDist = VecT(abs(A.m_Center.x - B.m_Center.x),
        abs(A.m_Center.y - B.m_Center.y));
    const VecT MaxDist = VecT(abs(A.m_HalfSize.x - B.m_HalfSize.x),
        abs(A.m_HalfSize.y - B.m_HalfSize.y));
    return CenterDist.x <= MaxDist.x && CenterDist.y <= MaxDist.y;
}

ci::Rectf ToRect( const CRI_AABB Box )
{
    using ci::Rectf;
    return Rectf(Box.m_Center - Box.m_HalfSize, Box.m_Center + Box.m_HalfSize);
}

void SetAABB( CRI_AABB& Box, const CRI_AABB::VecT Center,
    const CRI_AABB::VecT HalfSize )
{
    Box.m_Center = Center;
    Box.m_HalfSize = HalfSize;
}