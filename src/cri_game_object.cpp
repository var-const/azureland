#include "cri_stdafx.h"

#include "cri_game_object.h"

#include <cinder/gl/gl.h>

CRIGameObject::CRIGameObject( const SizeT& Size, const PosT& StartPos )
: CRIMovable(Size, StartPos)
, m_IsDead(false)
, m_pScene(0)
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

void CRIGameObject::SetScene( CRIGameScene& Scene )
{
    m_pScene = &Scene;
}

CRIGameScene& CRIGameObject::GetScene()
{
    assert(m_pScene);
    return *m_pScene;
}

const CRIGameScene& CRIGameObject::GetScene() const
{
    assert(m_pScene);
    return *m_pScene;
}
