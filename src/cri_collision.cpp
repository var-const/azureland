#include "cri_stdafx.h"

#include "cri_collision.h"

#include <utility>

CRICollision::CRICollision()
: m_pObjA(NULL)
, m_pObjB(NULL)
, m_Time(0.f)
{
}

#ifdef PASS_BY_VALUE
CRICollisionsInfo::CRICollisionsInfo( const float Time,
    const CollisionsIterT Begin, const CollisionsIterT End )
#else
CRICollisionsInfo::CRICollisionsInfo( const float Time,
    CollisionsIterT const& Begin, CollisionsIterT const& End )
#endif
: m_Time(Time)
, m_Begin(Begin)
, m_End(End)
{
}

bool CRICollisionsInfo::IsEmpty() const
{
    return m_Begin == m_End;
}

int CRICollisionsInfo::GetSize() const
{
    using std::distance;
    return distance(m_Begin, m_End);
}
