#pragma once

class CRIEnemy;
class CRIPlayer;
struct CRICollision;

void OnCollision(CRIEnemy& Lhs, CRIPlayer& Rhs, const CRICollision& Collision);
void OnCollision(CRIEnemy& Lhs, CRIEnemy& Rhs, const CRICollision& Collision);
