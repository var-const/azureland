#pragma once

class CRIEnemy;
class CRIPlayer;

void OnCollision(CRIEnemy& Enemy, CRIPlayer& Player);
void OnCollision(CRIEnemy& Lhs, CRIEnemy& Rhs);
