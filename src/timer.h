#pragma once

#include <cinder/Timer.h>

class CountdownTimer {
  public:
    void SetExpiresFromNow(double Seconds);
    double ExpiresFromNow() const;

    bool IsExpired() const;

  private:
    double m_EndTime{};
    ci::Timer m_Timer;
};
