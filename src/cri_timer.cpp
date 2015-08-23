#include "cri_stdafx.h"

#include "cri_timer.h"
#include <SDL/SDL_timer.h>

void Timer::start() { ticks_at_init_ = SDL_GetTicks(); }
auto Timer::get_ticks_elapsed() const { return SDL_GetTicks() - ticks_at_init_; }

CRICountdownTimer::CRICountdownTimer()
: m_EndTime(0)
{
}

bool CRICountdownTimer::IsExpired() const
{
    return m_Timer.getSeconds() >= m_EndTime;
}

void CRICountdownTimer::SetExpiresFromNow(CRICountdownTimer::SecondsT const Seconds)
{
    m_EndTime = sec_to_ticks(Seconds);
    timer_.start();
}

auto CRICountdownTimer::ExpiresFromNow() const
{
    return ticks_to_sec(m_EndTime) - get_seconds_elapsed(timer_);
}
