#include "stdafx.h"

#include "reload.h"

#include <cassert>

void CRIReload::OnShot()
{
    assert(IsReady());
    m_Timer.SetExpiresFromNow(m_ReloadTime);
}

bool CRIReload::IsReady() const { return m_Timer.IsExpired(); }

void CRIReload::SetReloadTime(const int Milliseconds)
{
    assert(Milliseconds >= 0);
    m_ReloadTime = static_cast<double>(Milliseconds) / 1000.0;
}
