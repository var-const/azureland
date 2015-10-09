#pragma once

#include <cinder/Timer.h>

class CRICountdownTimer
{
public:
    void SetExpiresFromNow(double Seconds);
    double ExpiresFromNow() const;

    bool IsExpired() const;

private:
    double m_EndTime{};
    ci::Timer m_Timer;
};
