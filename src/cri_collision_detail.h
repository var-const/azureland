#pragma once

struct CRICollision;
class CRIGameObject;

CRICollision CreateCollision( CRIGameObject& Lhs, CRIGameObject& Rhs,
    float Time );
float GetCollisionTime(CRIGameObject& Lhs, CRIGameObject& Rhs);
