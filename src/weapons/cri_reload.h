#pragma once

#include "../cri_timer.h"

class CRIReload
{
public:
    void SetReloadTime(int Milliseconds);
    void OnShot();
    bool IsReady() const;

private:
    double m_ReloadTime{};
    CRICountdownTimer m_Timer;
};
