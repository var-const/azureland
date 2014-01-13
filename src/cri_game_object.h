#pragma once

#include "cri_movable.h"

class CRIGameScene;

class CRIGameObject : public CRIMovable
{
public:	
    CRIGameObject(const SizeT& Size, const PosT& StartPos);

    void Draw();
    void Update(float Dt);

    void Destroy();
    bool IsDying() const; // In case I have time for death animations
    bool IsDead() const;

    void SetScene(const CRIGameScene& Scene);

protected:
    CRIGameScene& GetScene();
    const CRIGameScene& GetScene() const;

private:
    virtual void OnDestroyed();

    bool m_IsDead;

    CRIGameScene* m_pScene; // Non-owning
};
