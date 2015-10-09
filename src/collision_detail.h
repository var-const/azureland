#pragma once

struct Collision;
class GameObject;
class Movable;

Collision CreateCollision(
    GameObject& Lhs, GameObject& Rhs, float Time);
float GetCollisionTime(Movable& Lhs, Movable& Rhs, float Time);
