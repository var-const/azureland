#include "cri_stdafx.h"

#include "cri_game_object.h"

#include <cinder/gl/gl.h>

void CRIGameObject::Draw()
{
    using namespace ci;
    // @TODO: textures
    gl::drawStrokedRect( ToRect(GetAABB()) );
}

void CRIGameObject::Update(const float Dt)
{
    // @TODO
}
