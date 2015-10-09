#pragma once

#include "movable.h"

#include <cinder/gl/Texture.h> // @TODO: move to cpp

class CRIGameScene;

class CRIGameObject : public CRIMovable {
  public:
    CRIGameObject(const SizeT& Size, const PosT& StartPos);

    void Draw();
    void Update(float Dt);

    void SetTextureDescriptor(int Descr);
    int GetTextureDescriptor() const;

    virtual void LogicUpdate(float Dt);

    void SetDying();
    void Destroy();
    bool IsDying() const;
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

    bool m_IsDead{};

    CRIGameScene* m_pScene{};

    // @TODO: InvalidDescriptor
    int m_TextureDescriptor{-1};
};

void Draw(
    const CRIGameObject& Object, float Angle, const ci::gl::Texture& Texture);
