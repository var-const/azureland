#pragma once

#include "cri_camera.h"
#include "cri_collider.h"

#include <vector>

namespace cinder { template <typename T> class Vec2; }

class CRIGameObject;

class CRIGameScene
{
public:	
    CRIGameScene(int Width, int Height);
    ~CRIGameScene();

    void Draw();
    void Update(float Dt);

    void AddObject(CRIGameObject& Object);
    void MoveCamera(cinder::Vec2<float> NewCenter);

private:
    // No need
    CRIGameScene(const CRIGameScene&);
    CRIGameScene& operator=(const CRIGameScene&);

    void UpdateObjects(float Dt);

    typedef std::vector<CRIGameObject*> ObjectsContT;
    typedef ObjectsContT::iterator ObjectsItT;

    ObjectsContT m_Objects;
    CRICollider m_Collider;
    CRICamera m_Camera;
};
