#include "stdafx.h"

#include "on_collision.h"

#include "collision.h"
#include "game_objects.h"
#include "on_collision.h"

#include <cstddef>

namespace {

template <class T> T* GetObjByType(GameObject& Base)
{
    if (typeid(Base) == typeid(T)) {
        return dynamic_cast<T*>(&Base);
    }
    return nullptr;
}

template <class T> T* GetObjByType(GameObject& Lhs, GameObject& Rhs)
{
    T* Result = GetObjByType<T>(Lhs);
    if (!Result) {
        Result = GetObjByType<T>(Rhs);
    }
    return Result;
}

template <class T, class U>
bool TryProcessCollision(GameObject& Lhs, GameObject& Rhs)
{
    T* pT = GetObjByType<T>(Lhs, Rhs);
    if (!pT) {
        return false;
    }
    U* pU =
        GetObjByType<U>(static_cast<GameObject*>(pT) == &Lhs ? Rhs : Lhs);
    if (!pU) {
        return false;
    }

    OnCollision(*pT, *pU);

    return true;
}

// Double dispatch using typeids. Not elegant, but easy to implement

void HandleCollision(GameObject& Lhs, GameObject& Rhs)
{
    if (TryProcessCollision<Enemy, Enemy>(Lhs, Rhs)) {
        return;
    }
    if (TryProcessCollision<Enemy, Player>(Lhs, Rhs)) {
        return;
    }
    if (TryProcessCollision<Enemy, Obstacle>(Lhs, Rhs)) {
        return;
    }
    if (TryProcessCollision<Player, Obstacle>(Lhs, Rhs)) {
        return;
    }
    if (TryProcessCollision<Enemy, Projectile>(Lhs, Rhs)) {
        return;
    }
    if (TryProcessCollision<Enemy, Forcefield>(Lhs, Rhs)) {
        return;
    }
    if (TryProcessCollision<Obstacle, Projectile>(Lhs, Rhs)) {
        return;
    }
    if (TryProcessCollision<Player, HealthPickup>(Lhs, Rhs)) {
        return;
    }
    if (TryProcessCollision<Enemy, HealthPickup>(Lhs, Rhs)) {
        return;
    }
}

} // unnamed

void HandleCollision(const Collision& Collision)
{
    HandleCollision(*Collision.m_pObjA, *Collision.m_pObjB);
}
