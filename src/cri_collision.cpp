#include "cri_stdafx.h"

#include "cri_collision.h"

#include <utility>

CRICollision::CRICollision()
: m_pObjA(NULL)
, m_pObjB(NULL)
, m_Time(0.f)
{
}

CRICollisionsInfo::CRICollisionsInfo( const float Time,
    const CollisionsIterT Begin, const CollisionsIterT End )
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
