#include "cri_stdafx.h"

#include "cri_game_object.h"

#include <cinder/gl/gl.h>

CRIGameObject::CRIGameObject( const SizeT& Size, const PosT& StartPos )
: CRIMovable(Size, StartPos)
, m_IsDead(false)
, m_pScene(NULL)
{ 
}

CRIGameObject::~CRIGameObject()
{
    m_pScene = NULL;
}

void CRIGameObject::Draw()
{
    using ci::gl::drawStrokedRect;
    // @TODO: textures
    drawStrokedRect( ToRect(GetAABB()) );
}

void CRIGameObject::Update(const float Dt)
{
    if (IsDying())
    {
        return;
    }
    Move(Dt);
    DoUpdate(Dt);
}

void CRIGameObject::Destroy()
{
    if (IsDead())
    {
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

void CRIGameObject::DoUpdate( const float Dt )
{ 
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
