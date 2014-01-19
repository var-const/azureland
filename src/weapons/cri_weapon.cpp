#include "cri_stdafx.h"

#include "cri_weapon.h"

#include <cinder/Vector.h>

#include <cassert>
#include <cstddef>

using ci::Vec2f;

CRIWeapon::CRIWeapon()
: m_pScene(NULL)
{ 
}

CRIWeapon::~CRIWeapon()
{
}

void CRIWeapon::Shoot( const Vec2f StartPos, const Vec2f TargetPos )
{
    DoShoot(StartPos, TargetPos);
}

void CRIWeapon::SetScene( CRIGameScene& Scene )
{
    m_pScene = &Scene;
}

CRIGameScene& CRIWeapon::GetScene()
{
    assert(m_pScene);
    return *m_pScene;
}

const CRIGameScene& CRIWeapon::GetScene() const
{
    assert(m_pScene);
    return *m_pScene;
}
