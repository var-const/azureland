#include "cri_stdafx.h"

#include "cri_text_object.h"

#include <cinder/gl/gl.h>

using ci::ColorA;
using ci::Font;
using std::string;

CRITextObject::CRITextObject(const PosT& Pos)
  : CRIGameObject({}, Pos)
{
}

void CRITextObject::SetText(const string& Text) { m_Text = Text; }

void CRITextObject::DoDraw()
{
    using ci::gl::drawString;
    drawString(m_Text, GetCenterPos(), m_Color, m_Font);
}

void CRITextObject::SetFont(const Font& Font) { m_Font = Font; }

void CRITextObject::SetColor(const ColorA& Color) { m_Color = Color; }
