#include "cri_stdafx.h"

#include "cri_weapon.h"

#include "math_vector.h"

#include <cassert>
#include <cstddef>



CRIWeapon::CRIWeapon()
: m_pScene(NULL)
{ 
}

CRIWeapon::~CRIWeapon()
{
}

void CRIWeapon::SetReloadTime( const int Milliseconds )
{
    m_Reload.SetReloadTime(Milliseconds);
}

void CRIWeapon::Shoot( const Vec2f StartPos, const Vec2f TargetPos )
{
    if (!m_Reload.IsReady())
    {
        return;
    }
    DoShoot(StartPos, TargetPos);
    m_Reload.OnShot();
}

bool CRIWeapon::IsReady() const
{
    return m_Reload.IsReady();
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
