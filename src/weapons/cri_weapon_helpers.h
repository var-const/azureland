#pragma once

namespace cinder {
template <typename T> class Vec2;
}

cinder::Vec2<float> GetTargetVector(
    cinder::Vec2<float> StartPos, cinder::Vec2<float> TargetPos);
