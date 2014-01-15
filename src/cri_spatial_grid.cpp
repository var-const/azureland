#include "cri_stdafx.h"

#include "cri_game_object.h"
#include "cri_movable.h"
#include "cri_spatial_grid.h"

void CRISpatialGrid::SetParams(const Parameters Params) 
{
    using ci::Vec2f;

    m_CellSize = Vec2f(Params.SceneSize.x / Params.CellsCount.x,
        Params.SceneSize.y / Params.CellsCount.y);
    m_Cells.clear();
    m_Cells.reserve(Params.CellsCount.x * Params.CellsCount.y);
    const Vec2f HalfSize = m_CellSize / 2.f;
    for (int i = 0; i != Params.CellsCount.x; ++i)
    {
        for (int j = 0; j != Params.CellsCount.y; ++j)
        {
            m_Cells.push_back(Cell());
            const Vec2f Center = Vec2f(i * m_CellSize.x, j * m_CellSize.y) +
                HalfSize;
            m_Cells.back().m_AABB = CRI_AABB(Center, HalfSize);
        }
    }
}

void CRISpatialGrid::Reinit( const ObjIterT Begin, const ObjIterT End,
    const float Time )
{
    for (CellMutableIterT cell = m_Cells.begin(); cell != m_Cells.end();
        ++cell)
    {
        cell->m_Objects.clear();
    }

    for (ObjIterT i = Begin; i != End; ++i)
    {
        const CRI_AABB MovementAABB = GetMovementAABB(**i, Time);
        for (CellMutableIterT cell = m_Cells.begin(); cell != m_Cells.end();
            ++cell)
        {
            if (Intersect(MovementAABB, cell->m_AABB))
            {
                cell->m_Objects.push_back(*i);
            }
        }
    }
}

CRISpatialGrid::CellIterT CRISpatialGrid::CellsBegin() const
{
    return m_Cells.begin();
}

CRISpatialGrid::CellIterT CRISpatialGrid::CellsEnd() const
{
    return m_Cells.end();
}
