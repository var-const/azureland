#pragma once

struct CRICollision;
class CRIGameObject;
class CRIMovable;

CRICollision CreateCollision( CRIGameObject& Lhs, CRIGameObject& Rhs,
    float Time );
float GetCollisionTime(CRIMovable& Lhs, CRIMovable& Rhs, float Time);
