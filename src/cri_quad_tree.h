#pragma once

#include "cri_aabb.h"
#include "cri_quad_tree_node.h"

class CRIGameObject;

class CRIQuadTree
{
public:
    explicit CRIQuadTree(CRI_AABB Bounds, int MaxObjects = 4,
        int MaxDepth = 10);

    bool Insert(CRIGameObject* Val);
    bool Erase(CRIGameObject* Val);
    void Clear();

	template <typename OutputIt>
    OutputIt CopyIntersecting(CRI_AABB Box, OutputIt Iter) const;
    void Rearrange();
    void ShrinkToFit();

    // @TODO: size could be cached and updated on each successful insert/erase
    int Size() const;
    bool Empty() const;

    void Draw() const;
	template <typename OutputIt>
    OutputIt Copy(OutputIt Iter) const;

private:
	CRIQuadTreeNode m_Root;
};

//////////////////////////////////////////////////////////////////////////
// Impl
//////////////////////////////////////////////////////////////////////////

template <typename OutputIt>
OutputIt CRIQuadTree::CopyIntersecting(const CRI_AABB Box,
    const OutputIt Iter) const
{
    return m_Root.CopyIntersecting(Box, Iter);
}

template <typename OutputIt>
OutputIt CRIQuadTree::Copy(const OutputIt Iter) const
{
    return m_Root.Copy(Iter);
}
