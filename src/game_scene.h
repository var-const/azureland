#pragma once

#include "camera.h"
#include "collider.h"
#include "obstacle.h"
#include "pickup_generator.h"

#include <memory>
#include <vector>
#include <utility>

namespace cinder {
template <typename T> class Vec2;
}

class App;
class GameObject;

class GameScene {
  public:
    GameScene(App& App, int Width, int Height);

    void Draw();
    void Update(float Dt);

    void AddObject(std::unique_ptr<GameObject> Object);
	// @TODO: no need returning the object
    GameObject* AddGUIObject(std::unique_ptr<GameObject> Object);
    void MoveCamera(cinder::Vec2<float> NewCenter);

    void DestroyObject(GameObject& Obj);

    cinder::Vec2<int> GetSize() const;

    cinder::Vec2<float> ToScreenPos(cinder::Vec2<float> GamePos) const;
    cinder::Vec2<float> ToGamePos(cinder::Vec2<float> ScreenPos) const;

    const Collider& GetCollider() const;
    Camera& GetCamera();
    const Camera& GetCamera() const;

    void OnEnemyRespawn(cinder::Vec2<float> PosForPickUp);

    void EndGame(int Score);
    void SetPause(bool Val);

  private:
    // No need
    GameScene(const GameScene&);
    GameScene& operator=(const GameScene&);

    void AddPendingObjects();
    void RemoveDeadObjects();

    void UpdateObjects(float Dt);

    typedef std::vector<std::unique_ptr<GameObject>> ObjectsContT;

    ObjectsContT m_Objects;
    ObjectsContT m_PendingObjects;
    std::vector<GameObject*> m_DeadObjects;
    ObjectsContT m_GUIObjects;
    ObjectsContT m_PendingGUIObjects;

    Collider m_Collider;
    Camera m_Camera;

    std::pair<Obstacle, Obstacle> m_XBounds;
    std::pair<Obstacle, Obstacle> m_YBounds;

    PickupGenerator m_Pickups;

    bool m_IsPaused{};

    App* m_pApp{};
};
