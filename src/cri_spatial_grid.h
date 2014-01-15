#pragma once

#include "cri_aabb.h"

#include <cinder/Vector.h>

#include <vector>

class CRIGameObject;

class CRISpatialGrid
{
public:
    typedef std::vector<CRIGameObject*> ObjContT;
    typedef ObjContT::iterator ObjIterT;

    struct Parameters
    {
        ci::Vec2f SceneSize;
        ci::Vec2i CellsCount;
    };
    struct Cell
    {
        CRI_AABB m_AABB;
        ObjContT m_Objects;
    };

    typedef std::vector<Cell> CellsContT;
    typedef CellsContT::const_iterator CellIterT;

    CRISpatialGrid(ObjIterT Begin, ObjIterT End, float Time, Parameters Params);

    CellIterT CellsBegin() const;
    CellIterT CellsEnd() const;

private:
    typedef CellsContT::iterator CellMutableIterT;

    CellsContT m_Cells;
    ci::Vec2f m_CellSize;
};
