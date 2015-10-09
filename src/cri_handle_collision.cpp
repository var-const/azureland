#include "cri_stdafx.h"

#include "cri_on_collision.h"

#include "cri_collision.h"
#include "cri_game_objects.h"
#include "cri_on_collision.h"

#include <cstddef>

namespace {

template <class T>
T* GetObjByType( CRIGameObject& Base )
{
    if (typeid(Base) == typeid(T))
    {
        return dynamic_cast<T*>(&Base);
    }
    return nullptr;
}

template <class T>
T* GetObjByType( CRIGameObject& Lhs, CRIGameObject& Rhs )
{
    T* Result = GetObjByType<T>(Lhs);
    if (!Result)
    {
        Result = GetObjByType<T>(Rhs);
    }
    return Result;
}

template <class T, class U>
bool TryProcessCollision( CRIGameObject& Lhs, CRIGameObject& Rhs )
{
    T* pT = GetObjByType<T>(Lhs, Rhs);
    if (!pT)
    {
        return false;
    }
    U* pU = GetObjByType<U>(static_cast<CRIGameObject*>(pT) ==
        &Lhs ? Rhs : Lhs);
    if (!pU)
    {
        return false;
    }
    
    OnCollision(*pT, *pU);

    return true;
}

// Double dispatch using typeids. Not elegant, but easy to implement

void HandleCollision( CRIGameObject& Lhs, CRIGameObject& Rhs )
{
    if (TryProcessCollision<CRIEnemy, CRIEnemy>(Lhs, Rhs))
    {
        return;
    }
    if (TryProcessCollision<CRIEnemy, CRIPlayer>(Lhs, Rhs))
    {
        return;
    }
    if (TryProcessCollision<CRIEnemy, CRIObstacle>(Lhs, Rhs))
    {
        return;
    }
    if (TryProcessCollision<CRIPlayer, CRIObstacle>(Lhs, Rhs))
    {
        return;
    }
    if (TryProcessCollision<CRIEnemy, CRIProjectile>(Lhs, Rhs))
    {
        return;
    }
    if (TryProcessCollision<CRIEnemy, CRIForcefield>(Lhs, Rhs))
    {
        return;
    }
    if (TryProcessCollision<CRIObstacle, CRIProjectile>(Lhs, Rhs))
    {
        return;
    }
    if (TryProcessCollision<CRIPlayer, CRIHealthPickup>(Lhs, Rhs))
    {
        return;
    }
    if (TryProcessCollision<CRIEnemy, CRIHealthPickup>(Lhs, Rhs))
    {
        return;
    }
}

} // unnamed

void HandleCollision(const CRICollision& Collision)
{
    HandleCollision(*Collision.m_pObjA, *Collision.m_pObjB);
}
