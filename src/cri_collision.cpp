#include "cri_stdafx.h"

#include "cri_collision.h"

CRICollision::CRICollision()
: m_pObjA(NULL)
, m_pObjB(NULL)
, m_Time(0.f)
, m_Depth(0.f)
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
