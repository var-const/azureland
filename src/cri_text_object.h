#pragma once

#include "cri_game_object.h"

#include <cinder/Color.h>
#include <cinder/Font.h>

#include <string>

class CRITextObject : public CRIGameObject
{
public:	
    explicit CRITextObject(const PosT& Pos);
    
    void SetText(const std::string& Text);
    void SetFont(const ci::Font& Font);
    void SetColor(const ci::ColorA& Color);

private:
    void DoDraw() override;

    std::string m_Text;
    ci::Font m_Font;
    ci::Color m_Color{1.f, 1.f, 1.f, 1.f};
};
