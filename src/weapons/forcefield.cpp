#include "stdafx.h"

#include "forcefield.h"

#include "player.h"

#include <cinder/Vector.h>
#include <cinder/gl/gl.h>

using ci::Vec2f;

Forcefield::Forcefield(const float Radius, const PosT& Pos,
    const int Time, const int Damage, const float PushForce,
    const Player& Player)
  : GameObject(SizeT(), Pos)
  , m_TimeLeft(static_cast<float>(Time) / 1000.f)
  , m_EffectTime(static_cast<float>(Time) / 1000.f)
  , m_Damage(Damage)
  , m_PushForce(PushForce)
  , m_TargetRadius(Radius)
  , m_pPlayer(&Player)
{
    SetTextureDescriptor(TextureDescr);
}

void Forcefield::LogicUpdate(const float Dt)
{
    m_TimeLeft -= Dt;
    if (m_TimeLeft < 0.f) {
        Destroy();
        return;
    }
    SetCenterPos(m_pPlayer->GetCenterPos());
    const float Scale = 1.f - m_TimeLeft / m_EffectTime;
    const float Size = m_TargetRadius * Scale * 2.f;
    SetSize(Vec2f(Size, Size));
    SetScale(Scale);
}

void Forcefield::DoDraw()
{
    using ci::gl::drawStrokedCircle;
    drawStrokedCircle(GetCenterPos(), GetSize().x / 2.f);
}

int Forcefield::GetDamage() const { return m_Damage; }

bool Forcefield::Affect(const Enemy& Enemy)
{
    using std::find;

    if (find(m_Affected.begin(), m_Affected.end(), &Enemy) !=
        m_Affected.end()) {
        return false;
    }

    m_Affected.push_back(&Enemy);
    return true;
}

Vec2f Forcefield::GetPushVector(const Vec2f ToPos) const
{
    Vec2f Result = ToPos - GetCenterPos();
    Result.safeNormalize();
    Result *= m_PushForce;
    return Result;
}

int Forcefield::TextureDescr = -1;
