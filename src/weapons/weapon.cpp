#include "stdafx.h"

#include "weapon.h"

#include <cinder/Vector.h>

#include <cassert>
#include <cstddef>

using ci::Vec2f;

void Weapon::SetReloadTime(const int Milliseconds)
{
    m_Reload.SetReloadTime(Milliseconds);
}

void Weapon::Shoot(const Vec2f StartPos, const Vec2f TargetPos)
{
    if (!m_Reload.IsReady()) {
        return;
    }
    DoShoot(StartPos, TargetPos);
    m_Reload.OnShot();
}

bool Weapon::IsReady() const { return m_Reload.IsReady(); }

void Weapon::SetScene(GameScene& Scene) { m_pScene = &Scene; }

GameScene& Weapon::GetScene()
{
    assert(m_pScene);
    return *m_pScene;
}

const GameScene& Weapon::GetScene() const
{
    assert(m_pScene);
    return *m_pScene;
}
