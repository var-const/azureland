#include "stdafx.h"

#include "text_object.h"

#include <cinder/gl/gl.h>

using ci::ColorA;
using ci::Font;
using std::string;

TextObject::TextObject(const PosT& Pos)
  : GameObject({}, Pos)
{
}

void TextObject::SetText(const string& Text) { m_Text = Text; }

void TextObject::DoDraw()
{
    using ci::gl::drawString;
    drawString(m_Text, GetCenterPos(), m_Color, m_Font);
}

void TextObject::SetFont(const Font& Font) { m_Font = Font; }

void TextObject::SetColor(const ColorA& Color) { m_Color = Color; }
