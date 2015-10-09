#include "stdafx.h"

#include "crosshair.h"

#include "camera.h"

CRICrosshair::CRICrosshair(const SizeT& Size, const PosT& Pos)
  : CRIGameObject(Size, Pos)
  , m_Texture(TextureFromAsset("crosshair.png"))
{
}

void CRICrosshair::DoDraw()
{
    using namespace ci::gl;

    pushModelView();
    translate(GetCenterPos() - m_Texture.getSize() / 2.f);
    draw(m_Texture);
    popModelView();
}
