#pragma once

#include <cinder/Vector.h>

#include <vector>

class CRIGameObject;

class CRICamera
{
public:	
    CRICamera(ci::Vec2i SceneSize, ci::Vec2i ViewSize);

    void Draw();

    void AddObject(CRIGameObject& Object);
    void Move(ci::Vec2f NewCenter);
    ci::Vec2f ToScreenPos( ci::Vec2f GamePos ) const;
    ci::Vec2f ToGamePos( ci::Vec2f ScreenPos ) const;

private:
    typedef std::vector<CRIGameObject*> ObjectsContT;
    typedef ObjectsContT::iterator ObjectsItT;

    ObjectsContT m_Objects;

    ci::Vec2f m_ViewHalfSize;
    ci::Vec2i m_HorizBounds;
    ci::Vec2i m_VertBounds;

    ci::Vec2f m_CurTranslation;
};
