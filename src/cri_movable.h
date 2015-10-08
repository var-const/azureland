#pragma once

#include "cri_aabb.h"

#include "math_vector.h"

#include <utility>
#include "cri_aabb_int.h"

// @TODO: it could make sense to extract size and pos to
// another class, because, for example, obstacles don't move

class CRIMovable
{
public:
    typedef Vec2f SizeT;
    typedef Vec2f PosT;
    typedef Vec2f VelT;
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

    Vec2f GetHalfSize() const;
    Vec2f GetXBounds() const;
    Vec2f GetYBounds() const;

#else
    BoxT const& GetAABB() const;
    CRI_AABBd const& GetMovementAABB() const;

    Vec2f const& GetHalfSize() const;
    Vec2f const& GetXBounds() const;
    Vec2f const& GetYBounds() const;

#endif

    bool IsMoving() const;

    float GetAngle() const;
    Vec2f GetScale() const;

    void CacheMovementAABB(Rectf Rect);

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
    Vec2f m_XBounds;
    Vec2f m_YBounds;

    float m_Angle;
    Vec2f m_Scale;

    bool m_IsMoving;
};

CRIMovable::PosT GetFuturePos(const CRIMovable& Object, float Delta);
void ShiftPos(CRIMovable& Object, CRIMovable::PosT Shift);
CRIMovable::BoxT GetMovementAABB(const CRIMovable& Object, float Delta);
std::pair<Vec2i, Vec2i> GetMovementBounds( CRIMovable& Object,
    float Delta);
