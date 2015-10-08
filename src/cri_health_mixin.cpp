#include "cri_stdafx.h"

#include "cri_health_mixin.h"

#include <cinder/CinderMath.h>

#include <algorithm>
#include <cassert>

CRIHealthMixin::CRIHealthMixin()
: m_CurVal(0)
, m_MaxVal(0)
{ 
}

CRIHealthMixin::~CRIHealthMixin()
{
}

void CRIHealthMixin::SetMaxHealth( const int Max )
{
    assert(Max >= 0);
    m_MaxVal = Max;
}

void CRIHealthMixin::ModifyHealth( const int Val )
{
    using ci::math; using std::max;

    const int OldVal = m_CurVal;
    m_CurVal = math<int>::clamp(m_CurVal + Val, 0, max(m_CurVal, m_MaxVal));
    OnHealthModified(m_CurVal, m_CurVal - OldVal);
    if (m_CurVal == 0)
    {
        OnHealthDepleted();
    }
}

void CRIHealthMixin::ForceSetHealthValue( const int Val )
{
    const int OldVal = m_CurVal;
    m_CurVal = Val;
    OnHealthModified(m_CurVal, m_CurVal - OldVal);
    if (m_CurVal <= 0)
    {
        OnHealthDepleted();
    }
}

int CRIHealthMixin::GetCurHealthValue() const
{
    return m_CurVal;
}

int CRIHealthMixin::GetMaxHealthValue() const
{
    return m_MaxVal;
}

void CRIHealthMixin::OnHealthModified( const int NewVal, const int Modifier )
{
}

bool HasFullHealth( const CRIHealthMixin& Mixin )
{
    return Mixin.GetCurHealthValue() >= Mixin.GetMaxHealthValue();
}
