#include "cri_stdafx.h"

#include "cri_game_object.h"
#include "cri_movable.h"
#include "cri_spatial_grid.h"

CRISpatialGrid::CRISpatialGrid( const ObjIterT Begin, const ObjIterT End,
    const float Time, const Parameters Params )
{
    using ci::Vec2f; using std::fill;

    m_CellSize = Vec2f(Params.SceneSize.x / Params.CellsCount.x,
        Params.SceneSize.y / Params.CellsCount.y);
    m_Cells.reserve(Params.CellsCount.x * Params.CellsCount.y);
    for (int i = 0; i != Params.CellsCount.x; ++i)
    {
        for (int j = 0; j != Params.CellsCount.y; ++j)
        {
            m_Cells.push_back(Cell());
            const Vec2f LeftUpper = Vec2f(i * m_CellSize.x, j * m_CellSize.y);
            const Vec2f RightLower = LeftUpper + m_CellSize;
            m_Cells.back().m_AABB = CRI_AABB(LeftUpper, RightLower);
        }
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
