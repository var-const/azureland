#include "cri_stdafx.h"

#include "cri_weapon_helpers.h"

#include "math_vector.h"



Vec2f GetTargetVector( const Vec2f StartPos, const Vec2f TargetPos )
{
    Vec2f Result = TargetPos - StartPos;
    Result.safeNormalize();
    return Result;
}
