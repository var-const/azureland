#include "stdafx.h"

#include "crosshair.h"

#include "camera.h"

Crosshair::Crosshair(const SizeT& Size, const PosT& Pos)
  : GameObject(Size, Pos)
  , m_Texture(TextureFromAsset("crosshair.png"))
{
}

void Crosshair::DoDraw()
{
    using namespace ci::gl;

    pushModelView();
    translate(GetCenterPos() - m_Texture.getSize() / 2.f);
    draw(m_Texture);
    popModelView();
}
