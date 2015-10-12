
#include "stdafx.h"
#include "collision.h"

#include <utility>

#ifdef PASS_BY_VALUE
CollisionsInfo::CollisionsInfo(
    const float Time, const Collision::CollisionsIterT Begin, const Collision::CollisionsIterT End)
#else
CollisionsInfo::CollisionsInfo(
    const float Time, Collision::CollisionsIterT const& Begin, Collision::CollisionsIterT const& End)
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
