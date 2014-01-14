#pragma once

#include "cri_collider.h"

#include <vector>

class CRIGameObject;

class CRIGameScene
{
public:	
    CRIGameScene();
    ~CRIGameScene();

    void Draw();
    void Update(float Dt);

    void AddObject(CRIGameObject& Object);

private:
    // No need
    CRIGameScene(const CRIGameScene&);
    CRIGameScene& operator=(const CRIGameScene&);

    void UpdateObjects(float Dt);

    typedef std::vector<CRIGameObject*> ObjectsContT;
    typedef ObjectsContT::iterator ObjectsItT;

    ObjectsContT m_Objects;
    CRICollider m_Collider;
};
