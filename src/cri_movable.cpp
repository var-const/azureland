#include "cri_stdafx.h"

#include "cri_movable.h"

#include "cri_math.h"

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

void CRIMovable::Move( const float delta )
{
    const VelT path = GetVelocity() * delta;
    const PosT new_pos = GetCenterPos() + path;
    SetCenterPos(new_pos);
}

const CRIMovable::BoxT& CRIMovable::GetAABBRef() const
{
    return m_ABBB;
}

CRIMovable::BoxT CRIMovable::GetAABB() const
{
    return m_ABBB;
}

void CRIMovable::UpdateBoundingBox()
{
    SetAABB(m_ABBB, m_Pos, m_Size / 2.f);
}

bool CRIMovable::IsMoving() const
{
    return m_IsMoving;
}