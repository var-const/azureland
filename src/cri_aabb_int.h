#pragma once

#include <cinder/Rect.h>
#include <cinder/Vector.h>

struct CRI_AABBd
{
    typedef ci::Vec2d VecT;

    CRI_AABBd();
#ifdef PASS_BY_VALUE
    explicit CRI_AABBd(ci::Rectf Rect);
    CRI_AABBd(VecT Center, VecT HalfSize);
#else
    explicit CRI_AABBd(ci::Rectf const& Rect);
    CRI_AABBd(VecT const& Center, VecT const& HalfSize);
#endif

    VecT m_Center;
    VecT m_HalfSize;
};

#ifdef PASS_BY_VALUE
bool Intersect(CRI_AABBd A, CRI_AABBd B);
bool TouchOrIntersect(CRI_AABBd A, CRI_AABBd B);
bool TouchOnly(CRI_AABBd A, CRI_AABBd B);

CRI_AABBd::VecT IntersectionDepth(CRI_AABBd A, CRI_AABBd B);
CRI_AABBd::VecT IntersectionNormal(CRI_AABBd A, CRI_AABBd B);
CRI_AABBd::VecT IntersectionNormal(CRI_AABBd A, CRI_AABBd B, CRI_AABBd::VecT depth);

bool AContainsB(CRI_AABBd A, CRI_AABBd B);

ci::Rectf ToRect(CRI_AABBd Box);
void SetAABB(CRI_AABBd& Box, CRI_AABBd::VecT Center, CRI_AABBd::VecT HalfSize);

CRI_AABBd::VecT GetLeftUpper(CRI_AABBd Box);
CRI_AABBd::VecT GetRightLower(CRI_AABBd Box);

#else
bool Intersect(CRI_AABBd const& A, CRI_AABBd const& B);
bool TouchOrIntersect(CRI_AABBd const& A, CRI_AABBd const& B);
bool TouchOnly(CRI_AABBd const& A, CRI_AABBd const& B);

CRI_AABBd::VecT IntersectionDepth(CRI_AABBd const& A, CRI_AABBd const& B);
CRI_AABBd::VecT IntersectionNormal(CRI_AABBd const& A, CRI_AABBd const& B);
CRI_AABBd::VecT IntersectionNormal(CRI_AABBd const& A, CRI_AABBd const& B, CRI_AABBd::VecT depth);

bool AContainsB(CRI_AABBd const& A, CRI_AABBd const& B);

ci::Rectf ToRect(CRI_AABBd const& Box);
void SetAABB(CRI_AABBd& Box, CRI_AABBd::VecT Center, CRI_AABBd::VecT HalfSize);

CRI_AABBd::VecT GetLeftUpper(CRI_AABBd const& Box);
CRI_AABBd::VecT GetRightLower(CRI_AABBd const& Box);
#endif