#pragma once

class CRIGameObject;
class CRIEnemy;
class CRIPlayer;

void HandleCollision(CRIGameObject& Lhs, CRIGameObject& Rhs);

void OnCollision( CRIEnemy& Lhs, CRIPlayer& Rhs );
void OnCollision( CRIEnemy& Lhs, CRIEnemy& Rhs );
