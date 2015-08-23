#pragma once

#include <cstdint>

class Timer {
public:
    using TicksT = std::uint32_t;
    using SecondsT = double;

    void start();
    TicksT get_ticks_elapsed() const;

private:
    TicksT ticks_at_init_{};
};

Timer::TicksT sec_to_ticks(Timer::SecondsT const seconds)
{
    return static_cast<Timer::TicksT>(seconds * Timer::SecondsT{1000.0});
}

Timer::SecondsT ticks_to_sec(Timer::TicksT const ticks)
{
    return static_cast<Timer::SecondsT>(ticks / Timer::SecondsT{1000.f});
}

Timer::SecondsT get_seconds_elapsed(const Timer& timer) const
    { return ticks_to_sec(timer.get_ticks_elapsed()); }

class CRICountdownTimer
{
public:
    using TicksT = Timer::TicksT;
    using SecondsT = Timer::SecondsT;

    CRICountdownTimer();

    // @TODO: double (seconds) -> int (mseconds)
    void SetExpiresFromNow(SecondsT Seconds);
    SecondsT ExpiresFromNow() const;

    bool IsExpired() const;

private:
    TicksT m_EndTime{};
    Timer timer_;
};
