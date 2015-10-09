
#include "stdafx.h"
#include "collision.h"

#include <utility>

#ifdef PASS_BY_VALUE
CollisionsInfo::CollisionsInfo(
    const float Time, const CollisionsIterT Begin, const CollisionsIterT End)
#else
CollisionsInfo::CollisionsInfo(
    const float Time, CollisionsIterT const& Begin, CollisionsIterT const& End)
#endif
  : m_Time(Time)
  , m_Begin(Begin)
  , m_End(End)
{
}

bool CollisionsInfo::IsEmpty() const { return m_Begin == m_End; }

int CollisionsInfo::GetSize() const
{
    using std::distance;
    return distance(m_Begin, m_End);
}
