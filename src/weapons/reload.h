#pragma once

#include "../timer.h"

class Reload {
  public:
    void SetReloadTime(int Milliseconds);
    void OnShot();
    bool IsReady() const;

  private:
    double m_ReloadTime{};
    CountdownTimer m_Timer;
};
