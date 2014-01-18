#pragma once 
#include "cri_aabb.h"
#include "cri_movable.h"

#include <cinder/Vector.h>

class CRIGameObject;

template <int MaxRows, int MaxCols>
class CRISpatialGrid
{
public:
    static const int RowsC = MaxRows;
    static const int ColsC = MaxCols;

    typedef std::vector<CRIGameObject*> ObjContT;
    typedef ObjContT::iterator ObjIterT;

    struct Cell
    {
        CRI_AABB m_AABB;
        ObjContT m_Objects;
    };

    typedef ObjContT CellsContT[MaxRows][MaxCols];

    CRISpatialGrid(int Width, int Height);
    void SetSize(ci::Vec2i Size);
    void Reinit(ObjIterT Begin, ObjIterT End, float Time);

    //CellsContT m_Cells; // @FIXME make iterator
    ObjContT m_Cells[RowsC][ColsC];

private:
    ci::Vec2i m_CellSize;
};


template <int MaxRows, int MaxCols>
CRISpatialGrid<MaxRows, MaxCols>::CRISpatialGrid( const int Width,
    const int Height )
{
    SetSize(ci::Vec2i(Width, Height));
}

template <int MaxRows, int MaxCols>
void CRISpatialGrid<MaxRows, MaxCols>::SetSize( const ci::Vec2i Size )
{
    using ci::Vec2i;
    m_CellSize = Size / Vec2i(MaxRows, MaxCols);
}

template <int MaxRows, int MaxCols>
void CRISpatialGrid<MaxRows, MaxCols>::Reinit( const ObjIterT Begin,
    const ObjIterT End, const float Time )
{
    using ci::Vec2i;
    using std::max; using std::min; using std::pair;
    
    assert(m_CellSize != Vec2i::zero());

    for (int Row = 0; Row != MaxRows; ++Row)
    {
        for (int Col = 0; Col != MaxCols; ++Col)
        {
            m_Cells[Row][Col].clear();
        }
    }

    for (ObjIterT ObjIter = Begin; ObjIter != End; ++ObjIter)
    {
        const pair<Vec2i, Vec2i> MovementBounds =
            GetMovementBounds(**ObjIter, Time);
        const Vec2i LeftUpper = MovementBounds.first / m_CellSize;
        const Vec2i RightLower = MovementBounds.second / m_CellSize;
        for (int Row = max(LeftUpper.y, 0); Row < min(MaxRows, RightLower.y + 1);
            ++Row)
        {
            for (int Col = max(LeftUpper.x, 0);
                Col < min(MaxCols, RightLower.x + 1); ++Col)
            {
                m_Cells[Row][Col].push_back(*ObjIter);
            }
        }
    }
}
