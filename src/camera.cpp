#include "stdafx.h"

#include "camera.h"

#include "collider.h"
#include "container_helpers.h"
#include "game_object.h"
#include "movable.h"

#include <cinder/CinderMath.h>
#include <cinder/ImageIo.h>
#include <cinder/app/App.h>
#include <cinder/gl/gl.h>

#include <algorithm>
#include <string>

using ci::Vec2f;
using ci::Vec2i;
using ci::gl::Texture;
using std::string;


Camera::Camera(const Vec2i SceneSize, const Vec2i ViewSize)
  : m_ViewHalfSize(ViewSize / 2.f)
  , m_HorizBounds(0.f, SceneSize.x - ViewSize.x)
  , m_VertBounds(0.f, SceneSize.y - ViewSize.y)
{
    // @FIXME hard coded
    m_CollisionsBuffer.resize(5000);
    m_Back = TextureFromAsset("back.png");
}

int Camera::RegisterTexture(const string& Id)
{
    using ci::loadImage;
    using ci::app::loadAsset;

    m_Textures.push_back(TextureFromAsset(Id));
    ResizeAtLeast(m_Buffers, static_cast<int>(m_Textures.size()));
    return m_Textures.size() - 1;
}

void Camera::Draw()
{
    using ::Draw;
    using namespace ci;

    gl::pushModelView();
    DrawBack();
    gl::popModelView();

    gl::pushModelView();
    gl::translate(m_CurTranslation);

    for (int TextureDescr = 0; TextureDescr != m_Textures.size();
         ++TextureDescr) {
        if (!m_Buffers[TextureDescr].empty()) {
            BindTexture(m_Textures[TextureDescr]);
        }
        for (auto&& buf : m_Buffers[TextureDescr]) {
            Draw(*buf, m_Textures[TextureDescr]);
        }
    }

    gl::popModelView();
}

void Camera::UpdateVisibility(const Collider& Collider)
{
    using std::sort;
    using std::unique;

    for (auto&& buf : m_Buffers) {
        buf.clear();
    }

    const Vec2i LeftUpper = -m_CurTranslation - Vec2f(1.f, 1.f);
    const Vec2i RightLower =
        -m_CurTranslation + m_ViewHalfSize * 2.f + Vec2f(1.f, 1.f);
    m_CollisionsEnd = Collider.CopyColliding(
        LeftUpper, RightLower, m_CollisionsBuffer.begin());

    sort(m_CollisionsBuffer.begin(), m_CollisionsEnd);
    m_CollisionsEnd = unique(m_CollisionsBuffer.begin(), m_CollisionsEnd);

    for (auto CollisionsIter = m_CollisionsBuffer.begin();
         CollisionsIter != m_CollisionsEnd; ++CollisionsIter) {
        const int TextureDescr = (*CollisionsIter)->GetTextureDescriptor();
        if (TextureDescr != -1) {
            m_Buffers[TextureDescr].push_back(*CollisionsIter);
        }
    }
}

#ifdef PASS_BY_VALUE
void Camera::Move(const Vec2f NewCenter)
#else
void Camera::Move(Vec2f const& NewCenter)
#endif
{
    using ci::math;

    const Vec2f Translation = NewCenter - m_ViewHalfSize;
    m_CurTranslation.x = -1.f * math<float>::clamp(Translation.x,
                                    m_HorizBounds.x, m_HorizBounds.y);
    m_CurTranslation.y = -1.f * math<float>::clamp(Translation.y,
                                    m_VertBounds.x, m_VertBounds.y);
}

#ifdef PASS_BY_VALUE
Vec2f Camera::ToScreenPos(const Vec2f GamePos) const
#else
Vec2f Camera::ToScreenPos(Vec2f const& GamePos) const
#endif
{
    return GamePos + m_CurTranslation;
}

#ifdef PASS_BY_VALUE
Vec2f Camera::ToGamePos(const Vec2f ScreenPos) const
#else
Vec2f Camera::ToGamePos(Vec2f const& ScreenPos) const
#endif
{
    return ScreenPos - m_CurTranslation;
}

Vec2i Camera::GetSize() const { return Vec2i(m_HorizBounds.y, m_VertBounds.y); }

void Camera::DrawBack()
{
    using namespace ci;

    m_Back.enableAndBind();

    Rectf rect(0.f, 0.f, m_ViewHalfSize.x * 2.f, m_ViewHalfSize.y * 2.f);
    glEnableClientState(GL_VERTEX_ARRAY);
    GLfloat verts[8];
    glVertexPointer(2, GL_FLOAT, 0, verts);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    GLfloat texCoords[8];
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
    verts[0 * 2 + 0] = rect.getX2();
    verts[0 * 2 + 1] = rect.getY1();
    verts[1 * 2 + 0] = rect.getX1();
    verts[1 * 2 + 1] = rect.getY1();
    verts[2 * 2 + 0] = rect.getX2();
    verts[2 * 2 + 1] = rect.getY2();
    verts[3 * 2 + 0] = rect.getX1();
    verts[3 * 2 + 1] = rect.getY2();

    const Vec2f LeftUpper = -m_CurTranslation - Vec2f(1.f, 1.f);
    const Vec2f RightLower =
        -m_CurTranslation + m_ViewHalfSize * 2.f + Vec2f(1.f, 1.f);
    const Vec2f TextureSize = m_Back.getSize();
    const float x1 = LeftUpper.x / TextureSize.x;
    const float x2 = RightLower.x / TextureSize.x;
    const float y1 = LeftUpper.y / TextureSize.y;
    const float y2 = RightLower.y / TextureSize.y;

    texCoords[0 * 2 + 0] = x2;
    texCoords[0 * 2 + 1] = y1;
    texCoords[1 * 2 + 0] = x1;
    texCoords[1 * 2 + 1] = y1;
    texCoords[2 * 2 + 0] = x2;
    texCoords[2 * 2 + 1] = y2;
    texCoords[3 * 2 + 0] = x1;
    texCoords[3 * 2 + 1] = y2;

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void BindTexture(const Texture& Tex) { Tex.enableAndBind(); }

void Draw(const Movable& DrawData, const Texture& Tex) // @Remove second param
{
    using namespace ci;

    gl::pushModelView();

    gl::translate(DrawData.GetCenterPos());
    // gl::drawStrokedRect( ToRect(DrawData.GetAABB()) );
    gl::rotate(DrawData.GetAngle());
    gl::scale(DrawData.GetScale().x, DrawData.GetScale().y);
    gl::translate(-Tex.getSize() / 2.f);

    glEnableClientState(GL_VERTEX_ARRAY);
    GLfloat verts[8];
    glVertexPointer(2, GL_FLOAT, 0, verts);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    GLfloat texCoords[8];
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

    const ci::Rectf destRect = Tex.getCleanBounds();
    verts[0 * 2 + 0] = destRect.getX2();
    verts[0 * 2 + 1] = destRect.getY1();
    verts[1 * 2 + 0] = destRect.getX1();
    verts[1 * 2 + 1] = destRect.getY1();
    verts[2 * 2 + 0] = destRect.getX2();
    verts[2 * 2 + 1] = destRect.getY2();
    verts[3 * 2 + 0] = destRect.getX1();
    verts[3 * 2 + 1] = destRect.getY2();

    const ci::Rectf srcCoords = Tex.getAreaTexCoords(Tex.getCleanBounds());
    texCoords[0 * 2 + 0] = srcCoords.getX2();
    texCoords[0 * 2 + 1] = srcCoords.getY1();
    texCoords[1 * 2 + 0] = srcCoords.getX1();
    texCoords[1 * 2 + 1] = srcCoords.getY1();
    texCoords[2 * 2 + 0] = srcCoords.getX2();
    texCoords[2 * 2 + 1] = srcCoords.getY2();
    texCoords[3 * 2 + 0] = srcCoords.getX1();
    texCoords[3 * 2 + 1] = srcCoords.getY2();

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    gl::popModelView();
}

Texture TextureFromAsset(const string& Id)
{
    using ci::loadImage;
    using ci::app::loadAsset;
    return loadImage(loadAsset(Id));
}
