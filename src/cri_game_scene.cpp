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
    m_Objects.clear();
}

void CRIGameScene::Draw()
{
    for (ObjectsItT i = m_Objects.begin(); i != m_Objects.end(); ++i)
    {
        (*i)->Draw();
    }
}

void CRIGameScene::Update(const float Dt)
{
    ProcessCollisions(Dt);
}

void CRIGameScene::AddObject( CRIGameObject& Object )
{
    assert(std::find(m_Objects.begin(), m_Objects.end(), &Object) ==
        m_Objects.end());
    Object.SetScene(*this);
    m_Objects.push_back(&Object);
    // @FIXME would be called many times during initialization
    m_CollisionsBuffer.reserve(m_Objects.size());
}

void CRIGameScene::ProcessCollisions(float Dt)
{
    do
    {
        const float Time = m_Collider.BuildCollisions(m_CollisionsBuffer, Dt,
            m_Objects.begin(), m_Objects.end());
        if (m_CollisionsBuffer.empty())
        {
            break;
        }

        for (ObjectsItT i = m_Objects.begin(); i != m_Objects.end(); ++i)
        {
            (*i)->Update(Time);
        }
        Dt -= Time;
        for (??? i = m_CollisionsBuffer.begin(); i != m_CollisionsBuffer.end();
            ++i)
        {
            HandleCollision(*i);
        }
    }
    // Objects trajectory will change after collision, so we must take it
    // into account. @TODO: optimize?
    while (Dt > 0.f);

    if (Dt > 0.f)
    {
        for (ObjectsItT i = m_Objects.begin(); i != m_Objects.end(); ++i)
        {
            (*i)->Update(Dt);
        }
    }
}
