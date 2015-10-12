#include "stdafx.h"

#include "movable.h"

#include "math_helpers.h"

using ci::Vec2f;
using ci::Vec2i;
using std::pair;

Movable::Movable(const SizeT& Size, const PosT& StartPos)
  : m_Size(Size)
  , m_Pos(StartPos)
{
    UpdateBoundingBox();
}

Movable::Movable(const SizeT& Size, const PosT& StartPos, const VelT& Velocity)
  : m_Size(Size)
  , m_Pos(StartPos)
  , m_Velocity(Velocity)
{
    UpdateBoundingBox();
}

const Movable::VelT& Movable::GetVelocity() const { return m_Velocity; }

void Movable::SetVelocity(const VelT& V)
{
    m_IsMoving = !IsFpointEq(V.x, 0.f) || !IsFpointEq(V.y, 0.f);
    m_Velocity = V;
}

const Movable::PosT& Movable::GetCenterPos() const { return m_Pos; }

void Movable::SetCenterPos(const PosT& Pos)
{
    m_Pos = Pos;
    UpdateBoundingBox();
}

const Movable::SizeT& Movable::GetSize() const { return m_Size; }

void Movable::SetSize(const SizeT& Size)
{
    m_Size = Size;
    UpdateBoundingBox();
}

void Movable::Move(const float Delta)
{
    SetCenterPos(GetFuturePos(*this, Delta));
}

const Movable::BoxT& Movable::GetAABBRef() const { return m_AABB; }

#ifdef PASS_BY_VALUE
Movable::BoxT Movable::GetAABB() const
#else
Movable::BoxT const& Movable::GetAABB() const
#endif
{
    return m_AABB;
}

const AABBd& Movable::GetMovementAABBRef() const { return m_MovementAABB; }

#ifdef PASS_BY_VALUE
AABBd Movable::GetMovementAABB() const
#else
AABBd const& Movable::GetMovementAABB() const
#endif
{
    return m_MovementAABB;
}

void Movable::UpdateBoundingBox()
{
    using ::GetMovementAABB;
    SetAABB(m_AABB, m_Pos, m_Size / 2.f);
    m_XBounds =
        Vec2f(m_Pos.x - m_AABB.m_HalfSize.x, m_Pos.x + m_AABB.m_HalfSize.x);
    m_YBounds =
        Vec2f(m_Pos.y - m_AABB.m_HalfSize.y, m_Pos.y + m_AABB.m_HalfSize.y);
    // m_MovementAABB = GetMovementAABB(*this, 1.f / 60.f);
}

bool Movable::IsMoving() const { return m_IsMoving; }

void Movable::CacheMovementAABB(ci::Rectf Rect)
{
    m_MovementAABB = AABBd(Rect);
}

#ifdef PASS_BY_VALUE
Vec2f Movable::GetHalfSize() const
#else
Vec2f const& Movable::GetHalfSize() const
#endif
{
    return m_AABB.m_HalfSize;
}

#ifdef PASS_BY_VALUE
Vec2f Movable::GetXBounds() const
#else
Vec2f const& Movable::GetXBounds() const
#endif
{
    return m_XBounds;
}

#ifdef PASS_BY_VALUE
Vec2f Movable::GetYBounds() const
#else
Vec2f const& Movable::GetYBounds() const
#endif
{
    return m_YBounds;
}

void Movable::SetAngle(const float Deg) { m_Angle = Deg; }

float Movable::GetAngle() const { return m_Angle; }

void Movable::SetScale(const float Scale) { m_Scale.x = m_Scale.y = Scale; }

Vec2f Movable::GetScale() const { return m_Scale; }

Movable::PosT GetFuturePos(const Movable& Object, const float Delta)
{
    const Movable::VelT Path = Object.GetVelocity() * Delta;
    return Object.GetCenterPos() + Path;
}

void ShiftPos(Movable& Object, const Movable::PosT Shift)
{
    Object.SetCenterPos(Object.GetCenterPos() + Shift);
}

Movable::BoxT GetMovementAABB(const Movable& Object, const float Delta)
{
    using ci::Rectf;
    typedef Movable::BoxT BoxT;
    typedef Movable::PosT PosT;
    typedef Movable::SizeT SizeT;

    const SizeT HalfSize = Object.GetSize() / 2.f;
    const Rectf CurBox = Rectf(
        Object.GetCenterPos() - HalfSize, Object.GetCenterPos() + HalfSize);
    const PosT NewPos = GetFuturePos(Object, Delta);
    Rectf NewBox = Rectf(NewPos - HalfSize, NewPos + HalfSize);
    NewBox.include(CurBox);

    return BoxT(NewBox);
}

pair<Vec2i, Vec2i> GetMovementBounds(Movable& Object, float Delta)
{
    using ci::Rectf;
    using ci::Vec2f;
    using std::make_pair;
    typedef Movable::BoxT BoxT;
    typedef Movable::PosT PosT;
    typedef Movable::SizeT SizeT;

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
