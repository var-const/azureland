#include "stdafx.h"

#include "health_mixin.h"

#include <cinder/CinderMath.h>

#include <algorithm>
#include <cassert>

void HealthMixin::SetMaxHealth(const int Max)
{
    assert(Max >= 0);
    m_MaxVal = Max;
}

void HealthMixin::ModifyHealth(const int Val)
{
    using ci::math;
    using std::max;

    const int OldVal = m_CurVal;
    m_CurVal = math<int>::clamp(m_CurVal + Val, 0, max(m_CurVal, m_MaxVal));
    OnHealthModified(m_CurVal, m_CurVal - OldVal);
    if (m_CurVal == 0) {
        OnHealthDepleted();
    }
}

void HealthMixin::ForceSetHealthValue(const int Val)
{
    const int OldVal = m_CurVal;
    m_CurVal = Val;
    OnHealthModified(m_CurVal, m_CurVal - OldVal);
    if (m_CurVal <= 0) {
        OnHealthDepleted();
    }
}

int HealthMixin::GetCurHealthValue() const { return m_CurVal; }

int HealthMixin::GetMaxHealthValue() const { return m_MaxVal; }

void HealthMixin::OnHealthModified(const int NewVal, const int Modifier) {}

bool HasFullHealth(const HealthMixin& Mixin)
{
    return Mixin.GetCurHealthValue() >= Mixin.GetMaxHealthValue();
}
