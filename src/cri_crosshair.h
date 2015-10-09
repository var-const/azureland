#pragma once

#include "cri_game_object.h"

#include <cinder/gl/Texture.h>

class CRICrosshair : public CRIGameObject
{
public:	
    CRICrosshair(const SizeT& Size, const PosT& Pos);

private:
    void DoDraw() override;

    ci::gl::Texture m_Texture;
};
