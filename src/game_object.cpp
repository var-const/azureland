#include "stdafx.h"

#include "game_object.h"

#include "game_scene.h"

#include <cinder/gl/gl.h>

using ci::gl::Texture;

GameObject::GameObject(const SizeT& Size, const PosT& StartPos)
  : Movable(Size, StartPos)
{
}

void GameObject::Draw()
{
    using ci::gl::drawStrokedRect;
    // drawStrokedRect( ToRect(GetAABB()) );
    DoDraw();
}

void GameObject::Update(const float Dt)
{
    if (IsDying()) {
        return;
    }
    Move(Dt);
    DoUpdate(Dt);
}

void GameObject::Destroy()
{
    if (IsDead()) {
        return;
    }

    m_IsDead = true;
    GetScene().DestroyObject(*this);

    SetVelocity(VelT());
    OnDestroyed();
}

bool GameObject::IsDying() const { return m_IsDead; }

bool GameObject::IsDead() const { return m_IsDead; }

void GameObject::DoDraw() {}

void GameObject::DoUpdate(const float Dt) {}

void GameObject::LogicUpdate(const float Dt) {}

void GameObject::OnAddedToScene() {}

void GameObject::OnDestroyed() {}

void GameObject::SetScene(GameScene& Scene)
{
    m_pScene = &Scene;
    OnAddedToScene();
}

GameScene& GameObject::GetScene()
{
    assert(m_pScene);
    return *m_pScene;
}

const GameScene& GameObject::GetScene() const
{
    assert(m_pScene);
    return *m_pScene;
}

void GameObject::Ressurect()
{
    assert(m_IsDead);
    m_IsDead = false;
}

void GameObject::SetDying() { m_IsDead = true; }

void GameObject::SetTextureDescriptor(const int Descr)
{
    m_TextureDescriptor = Descr;
}

int GameObject::GetTextureDescriptor() const { return m_TextureDescriptor; }

void Draw(const GameObject& Object, const float Angle, const Texture& Texture)
{
    using namespace ci;

    gl::pushModelView();

    gl::translate(Object.GetCenterPos());
    gl::rotate(Angle);
    gl::translate(-Texture.getSize() / 2.f);

    Texture.enableAndBind();
    glEnableClientState(GL_VERTEX_ARRAY);
    GLfloat verts[8];
    glVertexPointer(2, GL_FLOAT, 0, verts);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    GLfloat texCoords[8];
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

    const Rectf destRect = Texture.getCleanBounds();
    verts[0 * 2 + 0] = destRect.getX2();
    verts[0 * 2 + 1] = destRect.getY1();
    verts[1 * 2 + 0] = destRect.getX1();
    verts[1 * 2 + 1] = destRect.getY1();
    verts[2 * 2 + 0] = destRect.getX2();
    verts[2 * 2 + 1] = destRect.getY2();
    verts[3 * 2 + 0] = destRect.getX1();
    verts[3 * 2 + 1] = destRect.getY2();

    const Rectf srcCoords = Texture.getAreaTexCoords(Texture.getCleanBounds());
    texCoords[0 * 2 + 0] = srcCoords.getX2();
    texCoords[0 * 2 + 1] = srcCoords.getY1();
    texCoords[1 * 2 + 0] = srcCoords.getX1();
    texCoords[1 * 2 + 1] = srcCoords.getY1();
    texCoords[2 * 2 + 0] = srcCoords.getX2();
    texCoords[2 * 2 + 1] = srcCoords.getY2();
    texCoords[3 * 2 + 0] = srcCoords.getX1();
    texCoords[3 * 2 + 1] = srcCoords.getY2();


    for (int i = 0; i != 2400; ++i) {
        gl::translate(Vec2f(10.f, 0.f));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        // gl::draw(Texture);
    }
    // for (int i = 0; i != 700; ++i) // 13
    //{
    //    gl::translate(Vec2f(10.f, 0.f));
    //    gl::draw(Texture);
    //    gl::draw(Object.m_Texture2);
    //}

    gl::popModelView();
}
