#include "cri_stdafx.h"

#include "cri_camera.h"

#include "cri_game_object.h"

#include <cinder/CinderMath.h>
#include <cinder/gl/gl.h>

using ci::Vec2f; using ci::Vec2i;

CRICamera::CRICamera( const Vec2i SceneSize, const Vec2i ViewSize )
: m_ViewHalfSize(ViewSize / 2.f)
, m_HorizBounds(0.f, SceneSize.x - ViewSize.x)
, m_VertBounds(0.f, SceneSize.y - ViewSize.y)
{ 
}

void CRICamera::Draw(const ObjectsItT Begin, const ObjectsItT End)
{
    using namespace ci;

    gl::pushModelView();
    //m_CurTranslation = Vec2f(200.f, 0.f);
    gl::translate(m_CurTranslation);

    for (ObjectsItT i = Begin; i != End; ++i)
    {
        (*i)->Draw();
    }

    gl::popModelView();
}

void CRICamera::Move( const Vec2f NewCenter )
{
    using ci::math;

    const Vec2f Translation = NewCenter - m_ViewHalfSize;
    m_CurTranslation.x = -1.f * math<float>::clamp(Translation.x, m_HorizBounds.x,
        m_HorizBounds.y);
    m_CurTranslation.y = -1.f * math<float>::clamp(Translation.y, m_VertBounds.x,
        m_VertBounds.y);
}

Vec2f CRICamera::ToScreenPos( const Vec2f GamePos ) const
{
    return GamePos + m_CurTranslation;
}

Vec2f CRICamera::ToGamePos( const Vec2f ScreenPos ) const
{
    return ScreenPos - m_CurTranslation;
}
