#include "stdafx.h"

#include "movable.h"

#include "math.h"

using ci::Vec2f;
using ci::Vec2i;
using std::pair;

CRIMovable::CRIMovable(const SizeT& Size, const PosT& StartPos)
  : m_Size(Size)
  , m_Pos(StartPos)
{
    UpdateBoundingBox();
}

CRIMovable::CRIMovable(
    const SizeT& Size, const PosT& StartPos, const VelT& Velocity)
  : m_Size(Size)
  , m_Pos(StartPos)
  , m_Velocity(Velocity)
{
    UpdateBoundingBox();
}

const CRIMovable::VelT& CRIMovable::GetVelocity() const { return m_Velocity; }

void CRIMovable::SetVelocity(const VelT& V)
{
    m_IsMoving = !IsFpointEq(V.x, 0.f) || !IsFpointEq(V.y, 0.f);
    m_Velocity = V;
}

const CRIMovable::PosT& CRIMovable::GetCenterPos() const { return m_Pos; }

void CRIMovable::SetCenterPos(const PosT& Pos)
{
    m_Pos = Pos;
    UpdateBoundingBox();
}

const CRIMovable::SizeT& CRIMovable::GetSize() const { return m_Size; }

void CRIMovable::SetSize(const SizeT& Size)
{
    m_Size = Size;
    UpdateBoundingBox();
}

void CRIMovable::Move(const float Delta)
{
    SetCenterPos(GetFuturePos(*this, Delta));
}

const CRIMovable::BoxT& CRIMovable::GetAABBRef() const { return m_AABB; }

#ifdef PASS_BY_VALUE
CRIMovable::BoxT CRIMovable::GetAABB() const
#else
CRIMovable::BoxT const& CRIMovable::GetAABB() const
#endif
{
    return m_AABB;
}

const CRI_AABBd& CRIMovable::GetMovementAABBRef() const
{
    return m_MovementAABB;
}

#ifdef PASS_BY_VALUE
CRI_AABBd CRIMovable::GetMovementAABB() const
#else
CRI_AABBd const& CRIMovable::GetMovementAABB() const
#endif
{
    return m_MovementAABB;
}

void CRIMovable::UpdateBoundingBox()
{
    using ::GetMovementAABB;
    SetAABB(m_AABB, m_Pos, m_Size / 2.f);
    m_XBounds =
        Vec2f(m_Pos.x - m_AABB.m_HalfSize.x, m_Pos.x + m_AABB.m_HalfSize.x);
    m_YBounds =
        Vec2f(m_Pos.y - m_AABB.m_HalfSize.y, m_Pos.y + m_AABB.m_HalfSize.y);
    // m_MovementAABB = GetMovementAABB(*this, 1.f / 60.f);
}

bool CRIMovable::IsMoving() const { return m_IsMoving; }

void CRIMovable::CacheMovementAABB(ci::Rectf Rect)
{
    m_MovementAABB = CRI_AABBd(Rect);
}

#ifdef PASS_BY_VALUE
Vec2f CRIMovable::GetHalfSize() const
#else
Vec2f const& CRIMovable::GetHalfSize() const
#endif
{
    return m_AABB.m_HalfSize;
}

#ifdef PASS_BY_VALUE
Vec2f CRIMovable::GetXBounds() const
#else
Vec2f const& CRIMovable::GetXBounds() const
#endif
{
    return m_XBounds;
}

#ifdef PASS_BY_VALUE
Vec2f CRIMovable::GetYBounds() const
#else
Vec2f const& CRIMovable::GetYBounds() const
#endif
{
    return m_YBounds;
}

void CRIMovable::SetAngle(const float Deg) { m_Angle = Deg; }

float CRIMovable::GetAngle() const { return m_Angle; }

void CRIMovable::SetScale(const float Scale) { m_Scale.x = m_Scale.y = Scale; }

Vec2f CRIMovable::GetScale() const { return m_Scale; }

CRIMovable::PosT GetFuturePos(const CRIMovable& Object, const float Delta)
{
    const CRIMovable::VelT Path = Object.GetVelocity() * Delta;
    return Object.GetCenterPos() + Path;
}

void ShiftPos(CRIMovable& Object, const CRIMovable::PosT Shift)
{
    Object.SetCenterPos(Object.GetCenterPos() + Shift);
}

CRIMovable::BoxT GetMovementAABB(const CRIMovable& Object, const float Delta)
{
    using ci::Rectf;
    typedef CRIMovable::BoxT BoxT;
    typedef CRIMovable::PosT PosT;
    typedef CRIMovable::SizeT SizeT;

    const SizeT HalfSize = Object.GetSize() / 2.f;
    const Rectf CurBox = Rectf(
        Object.GetCenterPos() - HalfSize, Object.GetCenterPos() + HalfSize);
    const PosT NewPos = GetFuturePos(Object, Delta);
    Rectf NewBox = Rectf(NewPos - HalfSize, NewPos + HalfSize);
    NewBox.include(CurBox);

    return BoxT(NewBox);
}

pair<Vec2i, Vec2i> GetMovementBounds(CRIMovable& Object, float Delta)
{
    using ci::Rectf;
    using ci::Vec2f;
    using std::make_pair;
    typedef CRIMovable::BoxT BoxT;
    typedef CRIMovable::PosT PosT;
    typedef CRIMovable::SizeT SizeT;

    const SizeT HalfSize = Object.GetSize() / 2.f;
    const Rectf CurBox = Rectf(
        Object.GetCenterPos() - HalfSize, Object.GetCenterPos() + HalfSize);
    const PosT NewPos = GetFuturePos(Object, Delta);
    Rectf NewBox = Rectf(NewPos - HalfSize, NewPos + HalfSize);
    NewBox.include(CurBox);
    const Vec2i LeftUpper = NewBox.getUpperLeft();
    const Vec2i RightLower = NewBox.getLowerRight() + Vec2f(1.f, 1.f);
    Object.CacheMovementAABB(NewBox);

    return make_pair(LeftUpper, RightLower);
}
