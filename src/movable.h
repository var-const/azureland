#pragma once

#include "aabb.h"

#include <cinder/Vector.h>

#include <utility>
#include "aabb_int.h"

// @TODO: it could make sense to extract size and pos to
// another class, because, for example, obstacles don't move

class Movable {
  public:
    typedef ci::Vec2f SizeT;
    typedef ci::Vec2f PosT;
    typedef ci::Vec2f VelT;
    typedef AABB BoxT;

    Movable(const SizeT& Size, const PosT& StartPos);
    Movable(const SizeT& Size, const PosT& StartPos, const VelT& Velocity);
    virtual ~Movable() {}

    void SetVelocity(const VelT& v);
    const VelT& GetVelocity() const;
    const PosT& GetCenterPos() const;
    void SetCenterPos(const PosT& Pos);
    const SizeT& GetSize() const;

    void Move(float Delta);

    const BoxT& GetAABBRef() const;
    const AABBd& GetMovementAABBRef() const;

#ifdef PASS_BY_VALUE
    BoxT GetAABB() const;
    AABBd GetMovementAABB() const;

    ci::Vec2f GetHalfSize() const;
    ci::Vec2f GetXBounds() const;
    ci::Vec2f GetYBounds() const;

#else
    BoxT const& GetAABB() const;
    AABBd const& GetMovementAABB() const;

    ci::Vec2f const& GetHalfSize() const;
    ci::Vec2f const& GetXBounds() const;
    ci::Vec2f const& GetYBounds() const;

#endif

    bool IsMoving() const;

    float GetAngle() const;
    ci::Vec2f GetScale() const;

    void CacheMovementAABB(ci::Rectf Rect);

  protected:
    void SetSize(const SizeT& Size);
    void SetAngle(float Deg);
    void SetScale(float Scale);

  private:
    // Cache the box to avoid recalculating it several times during the frame
    void UpdateBoundingBox();

    SizeT m_Size;
    PosT m_Pos;
    VelT m_Velocity;
    BoxT m_AABB;
    AABBd m_MovementAABB;
    ci::Vec2f m_XBounds;
    ci::Vec2f m_YBounds;

    float m_Angle{};
    ci::Vec2f m_Scale{1.f, 1.f};

    bool m_IsMoving{};
};

Movable::PosT GetFuturePos(const Movable& Object, float Delta);
void ShiftPos(Movable& Object, Movable::PosT Shift);
Movable::BoxT GetMovementAABB(const Movable& Object, float Delta);
std::pair<ci::Vec2i, ci::Vec2i> GetMovementBounds(Movable& Object, float Delta);
