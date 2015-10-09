#pragma once

#include <cinder/Rect.h>
#include <cinder/Vector.h>

struct AABBd {
    typedef ci::Vec2d VecT;

    AABBd() = default;
#ifdef PASS_BY_VALUE
    explicit AABBd(ci::Rectf Rect);
    AABBd(VecT Center, VecT HalfSize);
#else
    explicit AABBd(ci::Rectf const& Rect);
    AABBd(VecT const& Center, VecT const& HalfSize);
#endif

    VecT m_Center;
    VecT m_HalfSize;
};

#ifdef PASS_BY_VALUE
bool Intersect(AABBd A, AABBd B);
bool TouchOrIntersect(AABBd A, AABBd B);
bool TouchOnly(AABBd A, AABBd B);

AABBd::VecT IntersectionDepth(AABBd A, AABBd B);
AABBd::VecT IntersectionNormal(AABBd A, AABBd B);
AABBd::VecT IntersectionNormal(
    AABBd A, AABBd B, AABBd::VecT depth);

bool AContainsB(AABBd A, AABBd B);

ci::Rectf ToRect(AABBd Box);
void SetAABB(AABBd& Box, AABBd::VecT Center, AABBd::VecT HalfSize);

AABBd::VecT GetLeftUpper(AABBd Box);
AABBd::VecT GetRightLower(AABBd Box);

#else
bool Intersect(AABBd const& A, AABBd const& B);
bool TouchOrIntersect(AABBd const& A, AABBd const& B);
bool TouchOnly(AABBd const& A, AABBd const& B);

AABBd::VecT IntersectionDepth(AABBd const& A, AABBd const& B);
AABBd::VecT IntersectionNormal(AABBd const& A, AABBd const& B);
AABBd::VecT IntersectionNormal(
    AABBd const& A, AABBd const& B, AABBd::VecT depth);

bool AContainsB(AABBd const& A, AABBd const& B);

ci::Rectf ToRect(AABBd const& Box);
void SetAABB(AABBd& Box, AABBd::VecT Center, AABBd::VecT HalfSize);

AABBd::VecT GetLeftUpper(AABBd const& Box);
AABBd::VecT GetRightLower(AABBd const& Box);
#endif
