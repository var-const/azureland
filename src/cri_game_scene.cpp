#include "cri_stdafx.h"

#include "cri_game_scene.h"

#include "cri_collision.h"
#include "cri_game_object.h"
#include "cri_handle_collision.h"

#include <algorithm>
#include <cassert>
#include <map>
#include <utility>

using ci::Vec2f; using ci::Vec2i;

CRIGameScene::CRIGameScene(const int Width, const int Height)
: m_Collider(Width, Height)
, m_Camera(Vec2i(Width, Height), Vec2i(1280, 1024))
, m_XBounds(
    std::make_pair(
        CRIObstacle(Vec2f(1280 * 3, 50), Vec2f(1280.f * 3.f / 2.f, 25.f)),
        CRIObstacle(Vec2f(1280 * 3, 50), Vec2f(1280.f * 3.f / 2.f, 1024.f * 3.f - 25.f))
    )
)
, m_YBounds(
    std::make_pair(
        CRIObstacle(Vec2f(50, 1024 * 3), Vec2f(25.f, 1024.f * 3.f / 2.f)),
        CRIObstacle(Vec2f(50, 1024 * 3), Vec2f(1280.f * 3.f - 25.f, 1024.f * 3.f / 2.f))
    )
)
{ 
}

CRIGameScene::~CRIGameScene()
{
    for (ObjectsItT i = m_Objects.begin(); i != m_Objects.end(); ++i)
    {
        delete *i;
    }
    m_Objects.clear();

    for (ObjectsItT GUIObj = m_GUIObjects.begin(); GUIObj != m_GUIObjects.end();
        ++GUIObj)
    {
        delete *GUIObj;
    }
    m_GUIObjects.clear();
}

void CRIGameScene::Draw()
{
    m_Camera.Draw();

    for (ObjectsItT GUIObj = m_GUIObjects.begin(); GUIObj != m_GUIObjects.end();
        ++GUIObj)
    {
        (*GUIObj)->Draw();
    }

    //m_Collider.Draw();
}

void CRIGameScene::Update(const float Dt)
{
    UpdateObjects(Dt);

    for (ObjectsItT GUIObj = m_GUIObjects.begin(); GUIObj != m_GUIObjects.end();
        ++GUIObj)
    {
        (*GUIObj)->Update(Dt);
    }
}

void CRIGameScene::AddObject( CRIGameObject& Object )
{
    assert(std::find(m_Objects.begin(), m_Objects.end(), &Object) ==
        m_Objects.end());
    Object.SetScene(*this);
    m_Objects.push_back(&Object);
    // @FIXME would be called many times during initialization
    m_Collider.Reserve(m_Objects.size());
    m_Camera.AddObject(Object);
}

void CRIGameScene::AddGUIObject( CRIGameObject& Object )
{
    assert(std::find(m_GUIObjects.begin(), m_GUIObjects.end(), &Object) ==
        m_GUIObjects.end());
    Object.SetScene(*this);
    m_GUIObjects.push_back(&Object);
}

void CRIGameScene::UpdateObjects(float Dt)
{
    for (int i = 0; i != 10; ++i)
    {
        const CRICollisionsInfo Collisions = m_Collider.BuildCollisions(
            m_Objects.begin(), m_Objects.end(), Dt);
        if (Collisions.IsEmpty())
        {
            break;
        }

        if (Collisions.m_Time > 0.f)
        {
            for (ObjectsItT i = m_Objects.begin(); i != m_Objects.end(); ++i)
            {
                (*i)->Update(Collisions.m_Time);
            }
            Dt -= Collisions.m_Time;
        }
        for (CollisionsIterT i = Collisions.m_Begin; i != Collisions.m_End;
            ++i)
        {
            HandleCollision(*i);
        }
        if (Dt <= 0.f)
        {
            break;
        }
    }
    // Objects trajectory will change after collision, so we must take it
    // into account. @TODO: optimize?

    if (Dt > 0.f)
    {
        for (ObjectsItT i = m_Objects.begin(); i != m_Objects.end(); ++i)
        {
            (*i)->Update(Dt);
        }
    }

    for (ObjectsItT i = m_Objects.begin(); i != m_Objects.end(); ++i)
    {
        (*i)->LogicUpdate();
    }
} 

void CRIGameScene::MoveCamera( const Vec2f NewCenter )
{
    m_Camera.Move(NewCenter);
}

Vec2f CRIGameScene::ToScreenPos( const Vec2f GamePos ) const
{
    return m_Camera.ToScreenPos(GamePos);
}
