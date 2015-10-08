#pragma once

#include "math_vector.h"
#include <cinder/gl/Texture.h>

#include <vector>

class CRICollider;
class CRIGameObject;
class CRIMovable;

class CRICamera
{
public:	
    CRICamera(Vec2i SceneSize, Vec2i ViewSize);

    int RegisterTexture(const std::string& Id);
    void Draw();
    void UpdateVisibility(const CRICollider& Collider);

#ifdef PASS_BY_VALUE
    void Move(Vec2f NewCenter);
#else
    void Move(Vec2f const& NewCenter);
#endif

    Vec2i GetSize() const;
#ifdef PASS_BY_VALUE
    Vec2f ToScreenPos( Vec2f GamePos ) const;
    Vec2f ToGamePos( Vec2f ScreenPos ) const;
#else
    Vec2f ToScreenPos( Vec2f const& GamePos ) const;
    Vec2f ToGamePos( Vec2f const& ScreenPos ) const;
#endif

private:
    typedef std::vector<const CRIMovable*> DrawBufferT;
    typedef DrawBufferT::iterator DrawBufferIterT;
    typedef std::vector<DrawBufferT> BuffersContT;
    typedef BuffersContT::iterator BuffersIterT;

    typedef std::vector<CRIGameObject*> CollisionsBufferT;
    typedef CollisionsBufferT::iterator CollisionsIterT;

    void DrawBack();

    Vec2f m_ViewHalfSize;
    Vec2i m_HorizBounds;
    Vec2i m_VertBounds;

    Vec2f m_CurTranslation;

    std::vector<ci::gl::Texture> m_Textures;
    BuffersContT m_Buffers;

    CollisionsBufferT m_CollisionsBuffer;
    CollisionsIterT m_CollisionsEnd;

    ci::gl::Texture m_Back;
};

ci::gl::Texture TextureFromAsset(const std::string& Id);
void BindTexture(const ci::gl::Texture& Texture);
void Draw(const CRIMovable& DrawData, const ci::gl::Texture& Texture);
