#pragma once

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

    void ProcessCollisions(float Dt);

    typedef std::vector<CRIGameObject*> ObjectsContT;
    typedef ObjectsContT::iterator ObjectsItT;

    ObjectsContT m_Objects;
    Collider m_Collider;
    std::vector<Collision> m_CollisionsBuffer; // @TODO: move inside Collider
};
