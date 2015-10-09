#pragma once

#include "movable.h"

#include <cinder/gl/Texture.h> // @TODO: move to cpp

class GameScene;

class GameObject : public Movable {
  public:
    GameObject(const SizeT& Size, const PosT& StartPos);

    void Draw();
    void Update(float Dt);

    void SetTextureDescriptor(int Descr);
    int GetTextureDescriptor() const;

    virtual void LogicUpdate(float Dt);

    void SetDying();
    void Destroy();
    bool IsDying() const;
    bool IsDead() const;

    void SetScene(GameScene& Scene);

  protected:
    GameScene& GetScene();
    const GameScene& GetScene() const;

    void Ressurect();

  private:
    virtual void DoDraw();
    virtual void DoUpdate(float Dt);
    virtual void OnDestroyed();
    virtual void OnAddedToScene();

    bool m_IsDead{};

    GameScene* m_pScene{};

    // @TODO: InvalidDescriptor
    int m_TextureDescriptor{-1};
};

void Draw(
    const GameObject& Object, float Angle, const ci::gl::Texture& Texture);
