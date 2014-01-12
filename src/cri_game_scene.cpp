#include "cri_stdafx.h"

#include "cri_game_scene.h"

#include "cri_game_object.h"

#include <algorithm>
#include <cassert>

CRIGameScene::CRIGameScene()
{ 
}

CRIGameScene::~CRIGameScene()
{
    for (ObjectsItT i = m_Objects.begin(); i != m_Objects.end(); ++i)
    {
        delete *i;
    }
}

void CRIGameScene::Draw()
{
    for (ObjectsItT i = m_Objects.begin(); i != m_Objects.end(); ++i)
    {
        (*i)->Draw();
    }
}

void CRIGameScene::Update()
{
    for (ObjectsItT i = m_Objects.begin(); i != m_Objects.end(); ++i)
    {
        (*i)->Update();
    }
    ProcessCollisions();
}

void CRIGameScene::AddObject( CRIGameObject& Object )
{
    assert(std::find(m_Objects.begin(), m_Objects.end(), &Object) ==
        m_Objects.end());
    m_Objects.push_back(&Object);
}

void CRIGameScene::ProcessCollisions()
{
    // @TODO: m_Collider.Collide(m_Objects.begin(), m_Objects.end());
}
