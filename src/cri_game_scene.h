#pragma once

#include "cri_camera.h"
#include "cri_collider.h"
#include "cri_obstacle.h"
#include "cri_pickup_generator.h"

#include <vector>
#include <utility>

namespace cinder { template <typename T> class Vec2; }

class CRIApp;
class CRIGameObject;

class CRIGameScene
{
public:	
    CRIGameScene(CRIApp& App, int Width, int Height);
    ~CRIGameScene();

    void Draw();
    void Update(float Dt);

    void AddObject(CRIGameObject& Object);
    void AddGUIObject(CRIGameObject& Object);
    void MoveCamera(cinder::Vec2<float> NewCenter);

    void DestroyObject(CRIGameObject& Obj);

    cinder::Vec2<int> GetSize() const;

    cinder::Vec2<float> ToScreenPos(cinder::Vec2<float> GamePos) const;
    cinder::Vec2<float> ToGamePos(cinder::Vec2<float> ScreenPos) const;

    const CRICollider& GetCollider() const;
    CRICamera& GetCamera();
    const CRICamera& GetCamera() const;

    void OnEnemyRespawn(cinder::Vec2<float> PosForPickUp);

    void EndGame(int Score);
    void SetPause(bool Val);

private:
    // No need
    CRIGameScene(const CRIGameScene&);
    CRIGameScene& operator=(const CRIGameScene&);

    void AddPendingObjects();
    void RemoveDeadObjects();

    void UpdateObjects(float Dt);

    typedef std::vector<CRIGameObject*> ObjectsContT;
    typedef ObjectsContT::iterator ObjectsItT;

    ObjectsContT m_Objects;
    ObjectsContT m_PendingObjects;
    ObjectsContT m_DeadObjects;
    ObjectsContT m_GUIObjects;
    ObjectsContT m_PendingGUIObjects;

    CRICollider m_Collider;
    CRICamera m_Camera;

    std::pair<CRIObstacle, CRIObstacle> m_XBounds;
    std::pair<CRIObstacle, CRIObstacle> m_YBounds;

    CRIPickupGenerator m_Pickups;

    bool m_IsPaused;

    CRIApp* m_pApp; // Non-owning
};
