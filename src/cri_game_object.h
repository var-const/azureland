#pragma once

#include "cri_movable.h"

#include <cinder/gl/Texture.h> // @TODO: move to cpp

class CRIGameScene;

class CRIGameObject : public CRIMovable
{
public:	
    CRIGameObject(const SizeT& Size, const PosT& StartPos);
    ~CRIGameObject();

    void Draw();
    void Update(float Dt);

    void SetTexture(const ci::gl::Texture& Texture);

    virtual void LogicUpdate(float Dt);

    void SetDying();
    void Destroy();
    bool IsDying() const; // In case I have time for death animations
    bool IsDead() const;

    void SetScene(CRIGameScene& Scene);

protected:
    CRIGameScene& GetScene();
    const CRIGameScene& GetScene() const;

    void Ressurect();

private:
    virtual void DoDraw();
    virtual void DoUpdate(float Dt);
    virtual void OnDestroyed();
    virtual void OnAddedToScene();

    bool m_IsDead;

    CRIGameScene* m_pScene; // Non-owning
};

void Draw(const CRIGameObject& Object, float Angle, const ci::gl::Texture& Texture);
