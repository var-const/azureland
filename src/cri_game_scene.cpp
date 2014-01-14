#include "cri_stdafx.h"

#include "cri_game_scene.h"

#include "cri_collision.h"
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
    UpdateObjects(Dt);
}

void CRIGameScene::AddObject( CRIGameObject& Object )
{
    assert(std::find(m_Objects.begin(), m_Objects.end(), &Object) ==
        m_Objects.end());
    Object.SetScene(*this);
    m_Objects.push_back(&Object);
    // @FIXME would be called many times during initialization
    m_Collider.Reserve(m_Objects.size());
}

void CRIGameScene::UpdateObjects(float Dt)
{
    do
    {
        const CRICollisionsInfo Collisions = m_Collider.BuildCollisions(
            m_Objects.begin(), m_Objects.end(), Dt);
        if (Collisions.IsEmpty())
        {
            break;
        }

        for (ObjectsItT i = m_Objects.begin(); i != m_Objects.end(); ++i)
        {
            (*i)->Update(Collisions.m_Time);
        }
        Dt -= Collisions.m_Time;
        for (CollisionsIterT i = Collisions.m_Begin; i != Collisions.m_End;
            ++i)
        {
            //HandleCollision(*i); // @TODO:
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
