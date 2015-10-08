#pragma once

#include "cri_aabb.h"

#include <cinder/Vector.h>

#include <utility>
#include "cri_aabb_int.h"

// @TODO: it could make sense to extract size and pos to
// another class, because, for example, obstacles don't move

class CRIMovable
{
public:
    typedef ci::Vec2f SizeT;
    typedef ci::Vec2f PosT;
    typedef ci::Vec2f VelT;
    typedef CRI_AABB BoxT;

    CRIMovable(const SizeT& Size, const PosT& StartPos);
    CRIMovable(const SizeT& Size, const PosT& StartPos, const VelT& Velocity);
    virtual ~CRIMovable();

    void SetVelocity(const VelT& v);
    const VelT& GetVelocity() const;
    const PosT& GetCenterPos() const;
    void SetCenterPos(const PosT& Pos);
    const SizeT& GetSize() const;

    void Move(float Delta);

    const BoxT& GetAABBRef() const;
    const CRI_AABBd& GetMovementAABBRef() const;

#ifdef PASS_BY_VALUE
    BoxT GetAABB() const;
    CRI_AABBd GetMovementAABB() const;

    ci::Vec2f GetHalfSize() const;
    ci::Vec2f GetXBounds() const;
    ci::Vec2f GetYBounds() const;

#else
    BoxT const& GetAABB() const;
    CRI_AABBd const& GetMovementAABB() const;

    ci::Vec2f const& GetHalfSize() const;
    ci::Vec2f const& GetXBounds() const;
    ci::Vec2f const& GetYBounds() const;

#endif

    bool IsMoving() const;

    float GetAngle() const;
    ci::Vec2f GetScale() const;

    void CacheMovementAABB(ci::Rectf Rect);

protected:
    void SetSize(const SizeT& Size);
    void SetAngle(float Deg);
    void SetScale(float Scale);

private:
    // Cache the box to avoid recalculating it several times during the frame
    void UpdateBoundingBox();

    SizeT m_Size;
    PosT m_Pos;
    VelT m_Velocity;
    BoxT m_AABB;
    CRI_AABBd m_MovementAABB;
    ci::Vec2f m_XBounds;
    ci::Vec2f m_YBounds;

    float m_Angle;
    ci::Vec2f m_Scale;

    bool m_IsMoving;
};

CRIMovable::PosT GetFuturePos(const CRIMovable& Object, float Delta);
void ShiftPos(CRIMovable& Object, CRIMovable::PosT Shift);
CRIMovable::BoxT GetMovementAABB(const CRIMovable& Object, float Delta);
std::pair<ci::Vec2i, ci::Vec2i> GetMovementBounds( CRIMovable& Object,
    float Delta);
