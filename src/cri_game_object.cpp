#include "cri_stdafx.h"

#include "cri_game_object.h"

#include "cri_game_scene.h"

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
    GetScene().DestroyObject(*this);

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

void CRIGameObject::LogicUpdate()
{
}

void CRIGameObject::OnAddedToScene()
{ 
}

void CRIGameObject::OnDestroyed()
{ 
}

void CRIGameObject::SetScene( CRIGameScene& Scene )
{
    m_pScene = &Scene;
    OnAddedToScene();
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

void CRIGameObject::Ressurect()
{
    assert(m_IsDead);
    m_IsDead = false;
}

void CRIGameObject::SetDying()
{
    m_IsDead = true;
}
