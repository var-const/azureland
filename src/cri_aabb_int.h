#pragma once

#include <cinder/Rect.h>
#include <cinder/Vector.h>

struct CRI_AABBd
{
    typedef ci::Vec2d VecT;

    CRI_AABBd();
    explicit CRI_AABBd(ci::Rectf Rect);
    CRI_AABBd(VecT Center, VecT HalfSize);

    VecT m_Center;
    VecT m_HalfSize;
};

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
