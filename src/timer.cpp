#include "stdafx.h"

#include "timer.h"

bool CountdownTimer::IsExpired() const
{
    return m_Timer.getSeconds() >= m_EndTime;
}

void CountdownTimer::SetExpiresFromNow(const double Seconds)
{
    m_EndTime = Seconds;
    m_Timer.start();
}

double CountdownTimer::ExpiresFromNow() const
{
    return m_EndTime - m_Timer.getSeconds();
}
