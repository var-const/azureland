#pragma once

#include "game_object.h"

#include <cinder/gl/Texture.h>

class Crosshair : public GameObject {
  public:
    Crosshair(const SizeT& Size, const PosT& Pos);

  private:
    void DoDraw() override;

    ci::gl::Texture m_Texture;
};
