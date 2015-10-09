#pragma once

class Enemy;
class Forcefield;
class HealthPickup;
class Obstacle;
class Player;
class Projectile;

void OnCollision(Enemy& Enemy, Player& Player);
void OnCollision(Enemy& Lhs, Enemy& Rhs);
void OnCollision(Enemy& Lhs, Obstacle& Rhs);
void OnCollision(Player& Lhs, Obstacle& Rhs);
void OnCollision(Enemy& Lhs, Projectile& Rhs);
void OnCollision(Enemy& Lhs, Forcefield& Rhs);
void OnCollision(Obstacle& Lhs, Projectile& Rhs);
void OnCollision(Player& Lhs, HealthPickup& Rhs);
void OnCollision(Enemy& Lhs, HealthPickup& Rhs);
