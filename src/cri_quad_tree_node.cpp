#include "cri_stdafx.h"

#include "cri_quad_tree_node.h"

#include "cri_game_object.h"
#include "cri_movable.h"

#include <cinder/Color.h>
#include <cinder/Font.h>
#include <cinder/Rect.h>
#include <cinder/Vector.h>
#include <cinder/gl/gl.h>

#include <sstream>

CRIQuadTreeNode::CRIQuadTreeNode( const CRI_AABB Bounds, const int Depth,
    const LoadParams Params, CRIQuadTreeNode* Parent )
: m_AABB(Bounds)
, m_Depth(Depth)
, m_Params(Params)
, m_Parent(Parent)
{
}

CRIQuadTreeNode::CRIQuadTreeNode( const CRI_AABB Bounds, const int Depth,
    const int MaxDepth, const int MaxObjects, CRIQuadTreeNode* Parent )
: m_AABB(Bounds)
, m_Depth(Depth)
, m_Params(MaxDepth, MaxObjects)
, m_Parent(Parent)
{
}

bool CRIQuadTreeNode::Insert( CRIGameObject* const Val, const CRI_AABB ValBox )
{
    if (!AContainsB(m_AABB, ValBox))
    {
        return false;
    }
    if (MustSplit())
    {
        Split();
        PushObjectsToChildren();
    }
    if (InsertToChildren(Val, ValBox))
    {
        return true;
    }
    m_Objects.push_back(Val);

    return true;
}

bool CRIQuadTreeNode::Erase( CRIGameObject* const Val )
{
    // @FIXME partially duplicates EraseLocal
    using std::find;

    const ObjectsIterT Found = find(m_Objects.begin(), m_Objects.end(), Val);
    if (Found != m_Objects.end())
    {
        m_Objects.erase(Found);
        return true;
    }

    for (NodesIterT Node = m_Nodes.begin(); Node != m_Nodes.end(); ++Node)
    {
        if (Node->Erase(Val))
        {
            return true;
        }
    }

    return false;
}

void CRIQuadTreeNode::Clear()
{
    m_Objects.clear();
    m_Nodes.clear();
}

void CRIQuadTreeNode::Rearrange()
{
    for (ObjectsIterT ObjIter = m_Objects.begin(); ObjIter != m_Objects.end();
        /*omit*/)
    {
        CRIGameObject* Obj = *ObjIter;
        if (!Obj->IsMoving())
        {
            ++ObjIter;
            continue;
        }
        const CRI_AABB ValBox = Obj->GetMovementAABB();
        if (Pop(Obj, ValBox) || InsertToChildren(Obj, ValBox))
        { 
            ObjIter = EraseLocal(Obj);
        }
        else
        {
            ++ObjIter;
        }
    }

    for (NodesIterT Node = m_Nodes.begin(); Node != m_Nodes.end(); ++Node)
    {
        Node->Rearrange();
    }
}

void CRIQuadTreeNode::ShrinkToFit()
{
    bool NeedClear = true;
    for (NodesIterT Node = m_Nodes.begin(); Node != m_Nodes.end(); ++Node)
    {
        Node->ShrinkToFit();
        if (!Node->Empty())
        {
            NeedClear = false;
        }
    }
    if (NeedClear)
    {
        m_Nodes.clear();
    }
}

int CRIQuadTreeNode::Size() const
{
    int Sum = static_cast<int>(m_Objects.size());
    for (NodesConstIterT Node = m_Nodes.begin(); Node != m_Nodes.end(); ++Node)
    {
        Sum += static_cast<int>(Node->Size());
    }
    return Sum;
}

bool CRIQuadTreeNode::Empty() const
{
    if (!m_Objects.empty())
    {
        return false;
    }
    for (NodesConstIterT Node = m_Nodes.begin(); Node != m_Nodes.end(); ++Node)
    {
        if (!Node->Empty())
        {
            return false;
        }
    }
    return true;
}

void CRIQuadTreeNode::Draw() const
{
    {
        using namespace ci; using std::stringstream;

        const Rectf Bounds = ToRect(m_AABB);
        gl::SaveColorState State;
        gl::color(Color(1.f, 0.f, 0.f));
        gl::drawStrokedRect(Bounds);
        static stringstream Stream;
        Stream.str("");
        Stream << m_Objects.size();
        gl::drawString(Stream.str(),
            Bounds.getUpperLeft() + Vec2f(m_Depth * 50.f, 0.f),
            ColorA(1.f, 1.f, 1.f, 1.f), Font("Arial", 24));
    }

    for (NodesConstIterT Node = m_Nodes.begin(); Node != m_Nodes.end(); ++Node)
    {
        Node->Draw();
    }
}

bool CRIQuadTreeNode::Pop(CRIGameObject* const Val, const CRI_AABB ValBox)
{
    const bool NeedPop = !AContainsB(m_AABB, ValBox);
    if (NeedPop && m_Parent)
    {
        m_Parent->InsertOrPop(Val, ValBox);
    }
    return NeedPop;
}

void CRIQuadTreeNode::InsertOrPop(CRIGameObject* const Val, const CRI_AABB ValBox)
{
    if (!Pop(Val, ValBox))
    {
        Insert(Val, ValBox);
    }
}

CRIQuadTreeNode::ObjectsIterT CRIQuadTreeNode::EraseLocal(
    CRIGameObject* const Val)
{
    using std::find;

    const ObjectsIterT Found = find(m_Objects.begin(), m_Objects.end(), Val);
    return Found == m_Objects.end() ? Found : m_Objects.erase(Found);
}

bool CRIQuadTreeNode::HasChildren() const
{
    return !m_Nodes.empty();
}

bool CRIQuadTreeNode::MustSplit() const
{
    assert(m_Depth <= m_Params.m_MaxDepth);
    if (HasChildren())
    {
        return false;
    }
    return static_cast<int>(m_Objects.size()) >= m_Params.m_MaxObjects &&
        m_Depth != m_Params.m_MaxDepth;
}

void CRIQuadTreeNode::Split()
{
    using ci::Vec2f;

    m_Nodes.reserve(4);
    const Vec2f NewHorizSize = m_AABB.m_HalfSize / 2.f;
    const CRI_AABB BoxA = CRI_AABB(m_AABB.m_Center - NewHorizSize, NewHorizSize);
    m_Nodes.push_back(MyT(BoxA, m_Depth + 1, m_Params, this));
    const CRI_AABB BoxB = CRI_AABB(m_AABB.m_Center + NewHorizSize * Vec2f(1, -1),
        NewHorizSize);
    m_Nodes.push_back(MyT(BoxB, m_Depth + 1, m_Params, this));
    const CRI_AABB BoxC = CRI_AABB(m_AABB.m_Center + NewHorizSize * Vec2f(-1, 1),
        NewHorizSize);
    m_Nodes.push_back(MyT(BoxC, m_Depth + 1, m_Params, this));
    const CRI_AABB BoxD = CRI_AABB(m_AABB.m_Center + NewHorizSize, NewHorizSize);
    m_Nodes.push_back(MyT(BoxD, m_Depth + 1, m_Params, this));
}

void CRIQuadTreeNode::PushObjectsToChildren()
{
    for (ObjectsConstIterT ObjIter = m_Objects.begin();
        ObjIter != m_Objects.end(); /*omit*/)
    {
        if ( InsertToChildren(*ObjIter, (*ObjIter)->GetMovementAABB()) )
        {
            ObjIter = m_Objects.erase(ObjIter);
        }
        else
        {
            ++ObjIter;
        }
    }
}

bool CRIQuadTreeNode::InsertToChildren(CRIGameObject* const Val,
    const CRI_AABB ValBox)
{
    for (NodesIterT Node = m_Nodes.begin(); Node != m_Nodes.end(); ++Node)
    {
        if (Node->Insert(Val, ValBox))
        {
            return true;
        }
    }
    return false;
}

CRIQuadTreeNode::LoadParams::LoadParams()
: m_MaxDepth(0)
, m_MaxObjects(0)
{
}

CRIQuadTreeNode::LoadParams::LoadParams( const int Depth, const int Objects )
: m_MaxDepth(Depth)
, m_MaxObjects(Objects)
{
}
