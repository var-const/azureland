#include "cri_stdafx.h"

#include "cri_game_object.h"

#include <cinder/gl/gl.h>

CRIGameObject::CRIGameObject( const SizeT& Size, const PosT& StartPos )
: CRIMovable(Size, StartPos)
, m_IsDead()
{ 
}

CRIGameObject::CRIGameObject( const SizeT& Size, const PosT& StartPos,
    const VelT& Velocity )
: CRIMovable(Size, StartPos, Velocity)
, m_IsDead()
{ 
}

void CRIGameObject::Draw()
{
    using namespace ci;
    // @TODO: textures
    gl::drawStrokedRect( ToRect(GetAABB()) );
}

void CRIGameObject::Update(const float Dt)
{
    // @TODO
}

void CRIGameObject::Destroy()
{
    if (IsDead()) {
        return;
    }

    m_IsDead = true;
    SetVelocity(VelT());

    OnDestroyed();
}

bool CRIGameObject::IsDying() const
{
    return m_IsDead;
}

bool CRIGameObject::IsDead() const
{
    return m_IsDead;
}

void CRIGameObject::OnDestroyed()
{ 
}
