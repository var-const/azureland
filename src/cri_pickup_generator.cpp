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
    m_Camera.Draw(m_Objects.begin(), m_Objects.end());

    for (ObjectsItT GUIObj = m_GUIObjects.begin(); GUIObj != m_GUIObjects.end();
        ++GUIObj)
    {
        (*GUIObj)->Draw();
    }

    //m_Collider.Draw();
}

void CRIGameScene::Update(const float Dt)
{
    AddPendingObjects();
    RemoveDeadObjects();

    UpdateObjects(Dt);

    for (ObjectsItT GUIObj = m_GUIObjects.begin(); GUIObj != m_GUIObjects.end();
        ++GUIObj)
    {
        (*GUIObj)->Update(Dt);
    }
}

void CRIGameScene::AddPendingObjects()
{
    m_Objects.insert(m_Objects.end(), m_PendingObjects.begin(),
        m_PendingObjects.end());
    m_PendingObjects.clear();

    m_Collider.Reserve(m_Objects.size());

    m_GUIObjects.insert(m_GUIObjects.end(), m_PendingGUIObjects.begin(),
        m_PendingGUIObjects.end());
    m_PendingGUIObjects.clear();
}

void CRIGameScene::RemoveDeadObjects()
{
    using std::find;

    for (ObjectsItT Obj = m_DeadObjects.begin(); Obj != m_DeadObjects.end();
        ++Obj)
    {
        // Search from the end, because most of our objects never die,
        // and those that do are all appended at end during the game
        typedef ObjectsContT::reverse_iterator ReverseIterT;
        const ReverseIterT Found = find(m_Objects.rbegin(), m_Objects.rend(),
            *Obj); 
        assert(Found != m_Objects.rend());
        if (Found != m_Objects.rend())
        {
            delete *Found;
            m_Objects.erase((Found + 1).base());
        }
    }

    m_DeadObjects.clear();
}

void CRIGameScene::DestroyObject( CRIGameObject& Obj )
{
    m_DeadObjects.push_back(&Obj);
}

void CRIGameScene::AddObject( CRIGameObject& Object )
{
    assert(std::find(m_Objects.begin(), m_Objects.end(), &Object) ==
        m_Objects.end());
    Object.SetScene(*this);
    m_PendingObjects.push_back(&Object);
}

void CRIGameScene::AddGUIObject( CRIGameObject& Object )
{
    assert(std::find(m_GUIObjects.begin(), m_GUIObjects.end(), &Object) ==
        m_GUIObjects.end());
    Object.SetScene(*this);
    m_PendingGUIObjects.push_back(&Object);
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

Vec2f CRIGameScene::ToGamePos( const Vec2f ScreenPos ) const
{
    return m_Camera.ToGamePos(ScreenPos);
}

const CRICollider& CRIGameScene::GetCollider() const
{
    return m_Collider;
}

void CRIGameScene::OnEnemyDeath( const Vec2f PosForPickUp )
{
    m_Pickups.Invoke(PosForPickUp);
}
