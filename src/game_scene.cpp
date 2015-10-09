#include "stdafx.h"

#include "game_scene.h"

#include "app.h"
#include "collision.h"
#include "game_object.h"
#include "handle_collision.h"
#include "highscore.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <map>
#include <utility>

using ci::Vec2f;
using ci::Vec2i;

CRIGameScene::CRIGameScene(CRIApp& App, const int Width, const int Height)
  : m_Collider(Width, Height)
  , m_Camera(Vec2i(Width, Height), Vec2i(1280, 1024))
  , m_pApp(&App)
  , m_XBounds(std::make_pair(
        CRIObstacle(Vec2f(1280 * 3, 50), Vec2f(1280.f * 3.f / 2.f, 25.f)),
        CRIObstacle(Vec2f(1280 * 3, 50),
            Vec2f(1280.f * 3.f / 2.f, 1024.f * 3.f - 25.f))))
  , m_YBounds(std::make_pair(
        CRIObstacle(Vec2f(50, 1024 * 3), Vec2f(25.f, 1024.f * 3.f / 2.f)),
        CRIObstacle(Vec2f(50, 1024 * 3),
            Vec2f(1280.f * 3.f - 25.f, 1024.f * 3.f / 2.f))))
{
    m_Pickups.SetScene(*this);
}

void CRIGameScene::Draw()
{
    m_Camera.Draw();

    for (auto&& obj : m_GUIObjects) {
        obj->Draw();
    }

    // m_Collider.Draw();
}

void CRIGameScene::Update(const float Dt)
{
    AddPendingObjects();
    RemoveDeadObjects();

    if (!m_IsPaused) {
        UpdateObjects(Dt);
    }

    for (auto&& obj : m_GUIObjects) {
        obj->Update(Dt);
    }

    m_Camera.UpdateVisibility(m_Collider);
}

void CRIGameScene::AddPendingObjects()
{
    m_Objects.insert(m_Objects.end(),
        std::make_move_iterator(m_PendingObjects.begin()),
        std::make_move_iterator(m_PendingObjects.end()));
    m_PendingObjects.clear();

    m_Collider.Reserve(m_Objects.size());

    m_GUIObjects.insert(m_GUIObjects.end(),
        std::make_move_iterator(m_PendingGUIObjects.begin()),
        std::make_move_iterator(m_PendingGUIObjects.end()));
    m_PendingGUIObjects.clear();
}

void CRIGameScene::RemoveDeadObjects()
{
    using std::find;

    for (auto Obj = m_DeadObjects.begin(); Obj != m_DeadObjects.end(); ++Obj) {
        // Search from the end, because most of our objects never die,
        // and those that do are all appended at end during the game
        const auto Found = find(m_Objects.rbegin(), m_Objects.rend(), *Obj);
        assert(Found != m_Objects.rend());
        if (Found != m_Objects.rend()) {
            m_Objects.erase((Found + 1).base());
        }
    }

    m_DeadObjects.clear();
}

void CRIGameScene::DestroyObject(CRIGameObject& Obj)
{
    m_DeadObjects.push_back(&Obj);
}

void CRIGameScene::AddObject(std::unique_ptr<CRIGameObject> Object)
{
    Object.SetScene(*this);
    m_PendingObjects.push_back(std::move(Object));
}

CRIGameObject* CRIGameScene::AddGUIObject(std::unique_ptr<CRIGameObject> Object)
{
    Object.SetScene(*this);
    m_PendingGUIObjects.push_back(std::move(Object));
    return m_PendingGUIObjects.back().get();
}

void CRIGameScene::UpdateObjects(float Dt)
{
    const float FrameDt = Dt;

    for (int i = 0; i != 10; ++i) {
        const CRICollisionsInfo Collisions =
            m_Collider.BuildCollisions(m_Objects.begin(), m_Objects.end(), Dt);
        if (Collisions.IsEmpty()) {
            break;
        }

        if (Collisions.m_Time > 0.f) {
            for (auto&& obj : m_Objects) {
                obj->Update(Collisions.m_Time);
            }
            Dt -= Collisions.m_Time;
        }
        for (auto&& c : Collisions) {
            HandleCollision(c);
        }
        if (Dt <= 0.f) {
            break;
        }
    }
    // Objects trajectory will change after collision, so we must take it
    // into account. @TODO: optimize?

    if (Dt > 0.f) {
        for (auto&& obj : m_Objects) {
            obj->Update(Dt);
        }
    }

    for (auto&& obj : m_Objects) {
        obj->LogicUpdate(FrameDt);
    }
}

void CRIGameScene::MoveCamera(const Vec2f NewCenter)
{
    m_Camera.Move(NewCenter);
}

Vec2f CRIGameScene::ToScreenPos(const Vec2f GamePos) const
{
    return m_Camera.ToScreenPos(GamePos);
}

Vec2f CRIGameScene::ToGamePos(const Vec2f ScreenPos) const
{
    return m_Camera.ToGamePos(ScreenPos);
}

const CRICollider& CRIGameScene::GetCollider() const { return m_Collider; }

CRICamera& CRIGameScene::GetCamera() { return m_Camera; }

const CRICamera& CRIGameScene::GetCamera() const { return m_Camera; }

void CRIGameScene::OnEnemyRespawn(const Vec2f PosForPickUp)
{
    m_Pickups.Invoke(PosForPickUp);
}

Vec2i CRIGameScene::GetSize() const { return m_Camera.GetSize(); }

void CRIGameScene::EndGame(const int Score)
{
    SetPause(true);
    std::unique_ptr<CRIHighscore> Highscore = new CRIHighscore(Score);
    m_pApp->AddInputListener(*Highscore);
    AddGUIObject(std::move(Highscore));
    Highscore->Load("scores.yaml");
}

void CRIGameScene::SetPause(const bool Val) { m_IsPaused = Val; }
