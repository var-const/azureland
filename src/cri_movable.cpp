#include "cri_stdafx.h"

#include "cri_movable.h"

#include "cri_math.h"

using ci::Vec2i;
using std::pair;

CRIMovable::CRIMovable( const SizeT& Size, const PosT& StartPos )
: m_Size(Size)
, m_Pos(StartPos)
, m_IsMoving()
{
    UpdateBoundingBox();
}

CRIMovable::CRIMovable(const SizeT& Size, const PosT& StartPos,
    const VelT& Velocity)
: m_Size(Size)
, m_Pos(StartPos)
, m_Velocity(Velocity)
, m_IsMoving()
{
    UpdateBoundingBox();
}

CRIMovable::~CRIMovable() {}

const CRIMovable::VelT& CRIMovable::GetVelocity() const
{
    return m_Velocity;
}

void CRIMovable::SetVelocity(const VelT& V)
{
    m_IsMoving = !IsFpointEq(V.x, 0.f) || !IsFpointEq(V.y, 0.f);
    m_Velocity = V;
}

const CRIMovable::PosT& CRIMovable::GetCenterPos() const
{
    return m_Pos;
}

void CRIMovable::SetCenterPos( const PosT& Pos )
{
    m_Pos = Pos;
    UpdateBoundingBox();
}

const CRIMovable::SizeT& CRIMovable::GetSize() const
{
    return m_Size;
}

void CRIMovable::SetSize( const SizeT& Size )
{
    m_Size = Size;
    UpdateBoundingBox();
}

void CRIMovable::Move( const float Delta )
{
    SetCenterPos(GetFuturePos(*this, Delta));
}

const CRIMovable::BoxT& CRIMovable::GetAABBRef() const
{
    return m_AABB;
}

CRIMovable::BoxT CRIMovable::GetAABB() const
{
    return m_AABB;
}

const CRIMovable::BoxT& CRIMovable::GetMovementAABBRef() const
{
    return m_MovementAABB;
}

CRIMovable::BoxT CRIMovable::GetMovementAABB() const
{
    return m_MovementAABB;
}

void CRIMovable::UpdateBoundingBox()
{
    using ::GetMovementAABB;
    SetAABB(m_AABB, m_Pos, m_Size / 2.f);
    m_MovementAABB = GetMovementAABB(*this, 1.f / 60.f);
}

bool CRIMovable::IsMoving() const
{
    return m_IsMoving;
}

CRIMovable::PosT GetFuturePos( const CRIMovable& Object, const float Delta )
{
    const CRIMovable::VelT Path = Object.GetVelocity() * Delta;
    return Object.GetCenterPos() + Path;
}

void ShiftPos( CRIMovable& Object, const CRIMovable::PosT Shift )
{
    Object.SetCenterPos(Object.GetCenterPos() + Shift);
}

CRIMovable::BoxT GetMovementAABB( const CRIMovable& Object, const float Delta )
{
    using ci::Rectf;
    typedef CRIMovable::BoxT BoxT; typedef CRIMovable::PosT PosT;
    typedef CRIMovable::SizeT SizeT;

    const SizeT HalfSize = Object.GetSize() / 2.f;
    const Rectf CurBox = Rectf(Object.GetCenterPos() - HalfSize,
        Object.GetCenterPos() + HalfSize);
    const PosT NewPos = GetFuturePos(Object, Delta);
    Rectf NewBox = Rectf(NewPos - HalfSize, NewPos + HalfSize);
    NewBox.include(CurBox);

    return BoxT(NewBox);
}

pair<Vec2i, Vec2i> GetMovementBounds( const CRIMovable& Object, float Delta )
{
    using ci::Rectf; using ci::Vec2f;
    using std::make_pair;
    typedef CRIMovable::BoxT BoxT; typedef CRIMovable::PosT PosT;
    typedef CRIMovable::SizeT SizeT;

    const SizeT HalfSize = Object.GetSize() / 2.f;
    const Rectf CurBox = Rectf(Object.GetCenterPos() - HalfSize,
        Object.GetCenterPos() + HalfSize);
    const PosT NewPos = GetFuturePos(Object, Delta);
    Rectf NewBox = Rectf(NewPos - HalfSize, NewPos + HalfSize);
    NewBox.include(CurBox);
    const Vec2i LeftUpper = NewBox.getUpperLeft();
    const Vec2i RightLower = NewBox.getLowerRight() + Vec2f(1.f, 1.f);

    return make_pair(LeftUpper, RightLower);
}
