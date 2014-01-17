#include "cri_stdafx.h"

#include "cri_quad_tree.h"

#include "cri_game_object.h"

CRIQuadTree::CRIQuadTree(const CRI_AABB Bounds, const int MaxObjects,
    const int MaxDepth)
: m_Root(Bounds, 0, MaxDepth, MaxObjects)
{}

bool CRIQuadTree::Insert(CRIGameObject* const Val)
{
    //return m_Root.Insert(Val, Val->GetMovementAABB());
    return true;
}

bool CRIQuadTree::Erase(CRIGameObject* const Val)
{
    return m_Root.Erase(Val);
}

void CRIQuadTree::Clear()
{
    m_Root.Clear();
}

void CRIQuadTree::Draw() const
{
    m_Root.Draw();
}

void CRIQuadTree::Rearrange()
{
    m_Root.Rearrange();
}

void CRIQuadTree::ShrinkToFit()
{
    m_Root.ShrinkToFit();
}

int CRIQuadTree::Size() const
{
    return m_Root.Size();
}

bool CRIQuadTree::Empty() const
{
    return m_Root.Empty();
}
