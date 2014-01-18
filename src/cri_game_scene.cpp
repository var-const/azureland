#include "cri_stdafx.h"

#include "cri_game_scene.h"

#include "cri_collision.h"
#include "cri_game_object.h"
#include "cri_handle_collision.h"

#include <algorithm>
#include <cassert>

using ci::Vec2f; using ci::Vec2i;

CRIGameScene::CRIGameScene(const int Width, const int Height)
: m_Collider(Width, Height)
, m_Camera(Vec2i(Width, Height), Vec2i(1280, 1024))
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
    m_Camera.Draw();
    //m_Collider.Draw();
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
    m_Camera.AddObject(Object);
}

void CRIGameScene::UpdateObjects(float Dt)
{
    for (int i = 0; i != 5; ++i)
    {
        const CRICollisionsInfo Collisions = m_Collider.BuildCollisions(
            m_Objects.begin(), m_Objects.end(), Dt);
        if (Collisions.IsEmpty())
        {
            break;
        }
        const int Size = Collisions.GetSize();

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
            //assert( TouchOrIntersect(i->m_pObjA->GetAABB(),
            //    i->m_pObjB->GetAABB()) );
            HandleCollision(*i); // @TODO:
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
} 

void CRIGameScene::MoveCamera( const Vec2f NewCenter )
{
    m_Camera.Move(NewCenter);
}
