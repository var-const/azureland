#include "cri_stdafx.h"

#include "cri_forcefield.h"

#include "cri_player.h"

#include "math_vector.h"
#include <cinder/gl/gl.h>



CRIForcefield::CRIForcefield( const float Radius, const PosT& Pos, const int Time,
    const int Damage, const float PushForce, const CRIPlayer& Player )
: CRIGameObject(SizeT(), Pos)
, m_TimeLeft(static_cast<float>(Time) / 1000.f)
, m_EffectTime(static_cast<float>(Time) / 1000.f)
, m_Damage(Damage)
, m_PushForce(PushForce)
, m_TargetRadius(Radius)
, m_pPlayer(&Player)
{ 
    SetTextureDescriptor(TextureDescr);
}

void CRIForcefield::LogicUpdate(const float Dt)
{
    m_TimeLeft -= Dt;
    if (m_TimeLeft < 0.f)
    {
        Destroy();
        return;
    }
    SetCenterPos(m_pPlayer->GetCenterPos());
    const float Scale = 1.f - m_TimeLeft / m_EffectTime;
    const float Size = m_TargetRadius * Scale * 2.f;
    SetSize(Vec2f(Size, Size));
    SetScale(Scale);
}

void CRIForcefield::DoDraw()
{
    using ci::gl::drawStrokedCircle;
    drawStrokedCircle(GetCenterPos(), GetSize().x / 2.f);
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

Vec2f CRIForcefield::GetPushVector( const Vec2f ToPos ) const
{
    Vec2f Result = ToPos - GetCenterPos();
    Result.safeNormalize();
    Result *= m_PushForce;
    return Result;
}

int CRIForcefield::TextureDescr = -1;
