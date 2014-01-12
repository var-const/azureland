#pragma once

#include <vector>

class CRIGameObject;

class CRIGameScene
{
public:	
    CRIGameScene();
    ~CRIGameScene();

    void Draw();
    void Update();

    void AddObject(CRIGameObject& Object);

private:
    // No need
    CRIGameScene(const CRIGameScene&);
    CRIGameScene& operator=(const CRIGameScene&);

    void ProcessCollisions();

    typedef std::vector<CRIGameObject*> ObjectsContT;
    typedef ObjectsContT::iterator ObjectsItT;

    ObjectsContT m_Objects;
};
