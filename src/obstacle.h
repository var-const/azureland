#pragma once

#include "game_object.h"

class Obstacle : public GameObject {
  public:
    Obstacle(const SizeT& Size, const PosT& Pos);
};
