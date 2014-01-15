#pragma once

#include "cri_aabb.h"

#include <cinder/Vector.h>

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
    BoxT GetAABB() const;

    bool IsMoving() const;

protected:
    void SetSize(const SizeT& Size);

private:
    // Cache the box to avoid recalculating it several times during the frame
    void UpdateBoundingBox();

    SizeT m_Size;
    PosT m_Pos;
    VelT m_Velocity;
    BoxT m_ABBB;

    bool m_IsMoving;
};

CRIMovable::PosT GetFuturePos(const CRIMovable& Object, float Delta);
void ShiftPos(CRIMovable& Object, CRIMovable::PosT Shift);
CRIMovable::BoxT GetMovementAABB(const CRIMovable& Object, float Delta);