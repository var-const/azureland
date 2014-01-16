#pragma once

#include "cri_aabb.h"

#include <vector>

class CRIGameObject;

class CRIQuadTreeNode
{
    typedef CRIQuadTreeNode MyT;
    typedef std::vector<MyT> NodesContT;
    typedef NodesContT::iterator NodesIterT;
    typedef NodesContT::const_iterator NodesConstIterT;

    typedef std::vector<CRIGameObject*> ObjectsContT;
    typedef ObjectsContT::iterator ObjectsIterT;
    typedef ObjectsContT::const_iterator ObjectsConstIterT;

public:
    struct LoadParams
    {
        LoadParams();
        LoadParams(int Depth, int Objects);

        int m_MaxDepth;
        int m_MaxObjects;
    };

    CRIQuadTreeNode(CRI_AABB Bounds, int Depth, LoadParams Params,
        CRIQuadTreeNode* Parent = NULL);
    CRIQuadTreeNode(CRI_AABB Bounds, int Depth, int MaxDepth, int MaxObjects,
        CRIQuadTreeNode* Parent = NULL);

    bool Insert(CRIGameObject* const Val, CRI_AABB ValBox);
    bool Erase(CRIGameObject* const Val);
    void Clear();

    template <typename OutputIt>
    OutputIt CopyIntersecting(CRI_AABB Box, OutputIt Iter) const;

    void Rearrange();
    void ShrinkToFit();

    int Size() const;
    bool Empty() const;

	template <typename OutputIt>
    OutputIt Copy(OutputIt Iter) const;
    void Draw() const;

    static CRI_AABB Adapt(CRIGameObject* Val);
    static bool IntersectPred(CRI_AABB Lhs, CRIGameObject* Rhs);

private:
    template <typename OutputIt>
    OutputIt CopyIntersectingFromChildren(CRI_AABB Box, OutputIt Iter) const;

    bool Pop(CRIGameObject* Val, CRI_AABB ValBox);
    void InsertOrPop(CRIGameObject* Val, CRI_AABB ValBox);
    ObjectsIterT EraseLocal(CRIGameObject* Val);

    bool HasChildren() const;
    bool MustSplit() const;
    void Split();
    void PushObjectsToChildren();
    bool InsertToChildren(CRIGameObject* Val, CRI_AABB ValBox);

    ObjectsContT m_Objects;

    NodesContT m_Nodes;
    MyT* m_Parent;

    CRI_AABB m_AABB;

    int m_Depth;
    LoadParams m_Params;
};

//////////////////////////////////////////////////////////////////////////
// Impl
//////////////////////////////////////////////////////////////////////////

template <typename OutputIt>
OutputIt CRIQuadTreeNode::CopyIntersecting(const CRI_AABB Box,
    OutputIt Iter) const
{
    using std::copy;

    if (!Intersect(Box, m_AABB))
    {
        return Iter;
    }
    Iter = copy(m_Objects.begin(), m_Objects.end(), Iter);
    return CopyIntersectingFromChildren(Box, Iter);
}

template <typename OutputIt>
OutputIt CRIQuadTreeNode::Copy(OutputIt Iter) const
{
    using std::copy;

    Iter = copy(m_Objects.begin(), m_Objects.end(), Iter);
    for (NodesConstIterT Node = m_Nodes.begin(); Node != m_Nodes.end(); ++Node)
    {
        Iter = Node->Copy(Iter);
    }
    return Iter;
}

template <typename OutputIt>
OutputIt CRIQuadTreeNode::CopyIntersectingFromChildren(const CRI_AABB Box,
    OutputIt Iter) const
{
    for (NodesConstIterT Node = m_Nodes.begin(); Node != m_Nodes.end(); ++Node)
    {
        Iter = Node->CopyIntersecting(Box, Iter);
    }
    return Iter;
}
