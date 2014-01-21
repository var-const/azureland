#pragma once

#include <cinder/Vector.h>
#include <cinder/gl/Texture.h>

#include <vector>

class CRICollider;
class CRIGameObject;
class CRIMovable;

class CRICamera
{
public:	
    CRICamera(ci::Vec2i SceneSize, ci::Vec2i ViewSize);

    int RegisterTexture(const std::string& Id);
    void Draw();
    void UpdateVisibility(const CRICollider& Collider);

    void Move(ci::Vec2f NewCenter);

    ci::Vec2i GetSize() const;
    ci::Vec2f ToScreenPos( ci::Vec2f GamePos ) const;
    ci::Vec2f ToGamePos( ci::Vec2f ScreenPos ) const;

private:
    typedef std::vector<const CRIMovable*> DrawBufferT;
    typedef DrawBufferT::iterator DrawBufferIterT;
    typedef std::vector<DrawBufferT> BuffersContT;
    typedef BuffersContT::iterator BuffersIterT;

    typedef std::vector<CRIGameObject*> CollisionsBufferT;
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

void BindTexture(const ci::gl::Texture& Texture);
void Draw(const CRIMovable& DrawData, const ci::gl::Texture& Texture);
