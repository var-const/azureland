#pragma once

class CRIEnemy;
class CRIObstacle;
class CRIPlayer;

void OnCollision(CRIEnemy& Enemy, CRIPlayer& Player);
void OnCollision(CRIEnemy& Lhs, CRIEnemy& Rhs);
void OnCollision(CRIEnemy& Lhs, CRIObstacle& Rhs);
void OnCollision(CRIPlayer& Lhs, CRIObstacle& Rhs);
