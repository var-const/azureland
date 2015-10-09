#include "stdafx.h"

#include "timer.h"

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
