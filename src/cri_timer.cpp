#include "cri_stdafx.h"

#include "cri_timer.h"

CRICountdownTimer::CRICountdownTimer()
: m_EndTime(0)
{
}

bool CRICountdownTimer::IsExpired() const
{
    return m_Timer.getSeconds() >= m_EndTime;
}

void CRICountdownTimer::SetExpiresFromNow(const double Seconds)
{
    m_EndTime = Seconds;
    m_Timer.start();
}

double CRICountdownTimer::ExpiresFromNow() const
{
    return m_EndTime - m_Timer.getSeconds();
}
