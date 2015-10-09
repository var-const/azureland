#pragma once

#include <cinder/Vector.h>
#include <cinder/gl/Texture.h>

#include <vector>

class Collider;
class GameObject;
class Movable;

class Camera {
  public:
    Camera(ci::Vec2i SceneSize, ci::Vec2i ViewSize);

    int RegisterTexture(const std::string& Id);
    void Draw();
    void UpdateVisibility(const Collider& Collider);

#ifdef PASS_BY_VALUE
    void Move(ci::Vec2f NewCenter);
#else
    void Move(ci::Vec2f const& NewCenter);
#endif

    ci::Vec2i GetSize() const;
#ifdef PASS_BY_VALUE
    ci::Vec2f ToScreenPos(ci::Vec2f GamePos) const;
    ci::Vec2f ToGamePos(ci::Vec2f ScreenPos) const;
#else
    ci::Vec2f ToScreenPos(ci::Vec2f const& GamePos) const;
    ci::Vec2f ToGamePos(ci::Vec2f const& ScreenPos) const;
#endif

  private:
    typedef std::vector<const Movable*> DrawBufferT;
    typedef std::vector<DrawBufferT> BuffersContT;

    typedef std::vector<GameObject*> CollisionsBufferT;
    typedef CollisionsBufferT::iterator CollisionsIterT;

    void DrawBack();

    ci::Vec2f m_ViewHalfSize;
    ci::Vec2i m_HorizBounds;
    ci::Vec2i m_VertBounds;

    ci::Vec2f m_CurTranslation;

    std::vector<ci::gl::Texture> m_Textures;
    BuffersContT m_Buffers;

    CollisionsBufferT m_CollisionsBuffer;
    CollisionsIterT m_CollisionsEnd;

    ci::gl::Texture m_Back;
};

ci::gl::Texture TextureFromAsset(const std::string& Id);
void BindTexture(const ci::gl::Texture& Texture);
void Draw(const Movable& DrawData, const ci::gl::Texture& Texture);
