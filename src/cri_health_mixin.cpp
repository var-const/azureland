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

    m_CurVal = math<int>::clamp(m_CurVal + Val, 0, max(m_CurVal, m_MaxVal));
    if (m_CurVal == 0)
    {
        OnHealthDepleted();
    }
}

void CRIHealthMixin::ForceSetHealthValue( const int Val )
{
    m_CurVal = Val;
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
