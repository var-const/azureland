#include "cri_stdafx.h"

#include "cri_forcefield.h"

#include <cinder/Vector.h>
#include <cinder/gl/gl.h>

using ci::Vec2f;

CRIForcefield::CRIForcefield( const float Radius, const PosT& Pos, const int Time,
    const int Damage, const float PushForce, const float PushDecceleration )
: CRIGameObject(SizeT(), Pos)
, m_TimeLeft(static_cast<float>(Time) / 1000.f)
, m_EffectTime(static_cast<float>(Time) / 1000.f)
, m_Damage(Damage)
, m_PushForce(PushForce)
, m_PushDecceleration(PushDecceleration)
, m_TargetRadius(Radius)
{ 
}

void CRIForcefield::LogicUpdate(const float Dt)
{
    m_TimeLeft -= Dt;
    if (m_TimeLeft < 0.f)
    {
        Destroy();
        return;
    }
    const float Size = m_TargetRadius * (1.f - m_TimeLeft / m_EffectTime) * 2.f;
    SetSize(Vec2f(Size, Size));
}

void CRIForcefield::DoDraw()
{
    using ci::gl::drawStrokedCircle;
    drawStrokedCircle(GetCenterPos(), GetSize().x);
}

int CRIForcefield::GetDamage() const
{
    return m_Damage;
}

bool CRIForcefield::Affect( const CRIEnemy& Enemy )
{
    using std::find;
    if (find(m_Affected.begin(), m_Affected.end(), &Enemy) != m_Affected.end())
    {
        return false;
    }

    m_Affected.push_back(&Enemy);
    return true;
}
