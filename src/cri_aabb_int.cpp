#include "cri_stdafx.h"

#include "cri_aabb_int.h"

#include <cinder/Rect.h>

#include <cmath>

CRI_AABBd::CRI_AABBd()
{ 
}

#ifdef PASS_BY_VALUE
CRI_AABBd::CRI_AABBd( const VecT Center, const VecT HalfSize )
: m_Center(Center)
, m_HalfSize(HalfSize)
{
}

CRI_AABBd::CRI_AABBd( const ci::Rectf Rect )
: m_Center(Rect.getCenter())
, m_HalfSize(Rect.getSize() / 2 + ci::Vec2i(1, 1))
{
}

bool Intersect( const CRI_AABBd A, const CRI_AABBd B )
{
    using std::abs;
    return abs(A.m_Center.x - B.m_Center.x) < A.m_HalfSize.x + B.m_HalfSize.x &&
        abs(A.m_Center.y - B.m_Center.y) < A.m_HalfSize.y + B.m_HalfSize.y;
}

bool TouchOrIntersect( const CRI_AABBd A, const CRI_AABBd B )
{
    using std::abs;
    return abs(A.m_Center.x - B.m_Center.x) <=
        A.m_HalfSize.x + B.m_HalfSize.x &&
        abs(A.m_Center.y - B.m_Center.y) <=
        A.m_HalfSize.y + B.m_HalfSize.y;
}

bool TouchOnly( const CRI_AABBd A, const CRI_AABBd B )
{
    return TouchOrIntersect(A, B) && !Intersect(A, B);
}

CRI_AABBd::VecT IntersectionDepth( const CRI_AABBd A, const CRI_AABBd B )
{
    using std::abs;
    CRI_AABBd::VecT Dist = B.m_Center - A.m_Center;
    Dist.x = abs(Dist.x);
    Dist.y = abs(Dist.y);
    return A.m_HalfSize + B.m_HalfSize - Dist;
}

CRI_AABBd::VecT IntersectionNormal( const CRI_AABBd A, const CRI_AABBd B )
{
    return IntersectionNormal(A, B, IntersectionDepth(A, B));
}

CRI_AABBd::VecT IntersectionNormal( const CRI_AABBd A, const CRI_AABBd B,
    const CRI_AABBd::VecT Depth )
{
    CRI_AABBd::VecT Result;
    if (!(Depth.x < Depth.y)) {
        Result.y = (A.m_Center.y - B.m_Center.y > 0 ? 1 : -1);
    }
    if (!(Depth.y < Depth.x)) {
        Result.x = (A.m_Center.x - B.m_Center.x > 0 ? 1 : -1);
    }
    return Result;
}

bool AContainsB( const CRI_AABBd A, const CRI_AABBd B )
{
    using std::abs;
    typedef CRI_AABBd::VecT VecT;

    const VecT CenterDist = VecT(abs(A.m_Center.x - B.m_Center.x),
        abs(A.m_Center.y - B.m_Center.y));
    const VecT MaxDist = VecT(abs(A.m_HalfSize.x - B.m_HalfSize.x),
        abs(A.m_HalfSize.y - B.m_HalfSize.y));
    return CenterDist.x <= MaxDist.x && CenterDist.y <= MaxDist.y;
}

ci::Rectf ToRect( const CRI_AABBd Box )
{
    using ci::Rectf;
    return Rectf(Box.m_Center - Box.m_HalfSize, Box.m_Center + Box.m_HalfSize);
}

void SetAABB( CRI_AABBd& Box, const CRI_AABBd::VecT Center,
    const CRI_AABBd::VecT HalfSize )
{
    Box.m_Center = Center;
    Box.m_HalfSize = HalfSize;
}

CRI_AABBd::VecT GetLeftUpper( CRI_AABBd Box )
{
    return Box.m_Center - Box.m_HalfSize;
}

CRI_AABBd::VecT GetRightLower( CRI_AABBd Box )
{
    return Box.m_Center + Box.m_HalfSize;
}

#else
CRI_AABBd::CRI_AABBd( VecT const& Center, VecT const& HalfSize )
: m_Center(Center)
, m_HalfSize(HalfSize)
{
}

CRI_AABBd::CRI_AABBd( ci::Rectf const& Rect )
: m_Center(Rect.getCenter())
, m_HalfSize(Rect.getSize() / 2 + ci::Vec2i(1, 1))
{
}

bool Intersect( CRI_AABBd const& A, CRI_AABBd const& B )
{
    using std::abs;
    return abs(A.m_Center.x - B.m_Center.x) < A.m_HalfSize.x + B.m_HalfSize.x &&
        abs(A.m_Center.y - B.m_Center.y) < A.m_HalfSize.y + B.m_HalfSize.y;
}

bool TouchOrIntersect( CRI_AABBd const& A, CRI_AABBd const& B )
{
    using std::abs;
    return abs(A.m_Center.x - B.m_Center.x) <=
        A.m_HalfSize.x + B.m_HalfSize.x &&
        abs(A.m_Center.y - B.m_Center.y) <=
        A.m_HalfSize.y + B.m_HalfSize.y;
}

bool TouchOnly( CRI_AABBd const& A, CRI_AABBd const& B )
{
    return TouchOrIntersect(A, B) && !Intersect(A, B);
}

CRI_AABBd::VecT IntersectionDepth( CRI_AABBd const& A, CRI_AABBd const& B )
{
    using std::abs;
    CRI_AABBd::VecT Dist = B.m_Center - A.m_Center;
    Dist.x = abs(Dist.x);
    Dist.y = abs(Dist.y);
    return A.m_HalfSize + B.m_HalfSize - Dist;
}

CRI_AABBd::VecT IntersectionNormal( CRI_AABBd const& A, CRI_AABBd const& B )
{
    return IntersectionNormal(A, B, IntersectionDepth(A, B));
}

CRI_AABBd::VecT IntersectionNormal( CRI_AABBd const& A, CRI_AABBd const& B,
    CRI_AABBd::VecT Depth )
{
    CRI_AABBd::VecT Result;
    if (!(Depth.x < Depth.y)) {
        Result.y = (A.m_Center.y - B.m_Center.y > 0 ? 1 : -1);
    }
    if (!(Depth.y < Depth.x)) {
        Result.x = (A.m_Center.x - B.m_Center.x > 0 ? 1 : -1);
    }
    return Result;
}

bool AContainsB( CRI_AABBd const& A, CRI_AABBd const& B )
{
    using std::abs;
    typedef CRI_AABBd::VecT VecT;

    VecT CenterDist = VecT(abs(A.m_Center.x - B.m_Center.x),
        abs(A.m_Center.y - B.m_Center.y));
    VecT MaxDist = VecT(abs(A.m_HalfSize.x - B.m_HalfSize.x),
        abs(A.m_HalfSize.y - B.m_HalfSize.y));
    return CenterDist.x <= MaxDist.x && CenterDist.y <= MaxDist.y;
}

ci::Rectf ToRect( CRI_AABBd const& Box )
{
    using ci::Rectf;
    return Rectf(Box.m_Center - Box.m_HalfSize, Box.m_Center + Box.m_HalfSize);
}

void SetAABB( CRI_AABBd& Box, CRI_AABBd::VecT Center,
    CRI_AABBd::VecT HalfSize )
{
    Box.m_Center = Center;
    Box.m_HalfSize = HalfSize;
}

CRI_AABBd::VecT GetLeftUpper( CRI_AABBd const& Box )
{
    return Box.m_Center - Box.m_HalfSize;
}

CRI_AABBd::VecT GetRightLower( CRI_AABBd const& Box )
{
    return Box.m_Center + Box.m_HalfSize;
}
#endif
