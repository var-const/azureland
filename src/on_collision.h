#pragma once

class CRIEnemy;
class CRIForcefield;
class CRIHealthPickup;
class CRIObstacle;
class CRIPlayer;
class CRIProjectile;

void OnCollision(CRIEnemy& Enemy, CRIPlayer& Player);
void OnCollision(CRIEnemy& Lhs, CRIEnemy& Rhs);
void OnCollision(CRIEnemy& Lhs, CRIObstacle& Rhs);
void OnCollision(CRIPlayer& Lhs, CRIObstacle& Rhs);
void OnCollision(CRIEnemy& Lhs, CRIProjectile& Rhs);
void OnCollision(CRIEnemy& Lhs, CRIForcefield& Rhs);
void OnCollision(CRIObstacle& Lhs, CRIProjectile& Rhs);
void OnCollision(CRIPlayer& Lhs, CRIHealthPickup& Rhs);
void OnCollision(CRIEnemy& Lhs, CRIHealthPickup& Rhs);
