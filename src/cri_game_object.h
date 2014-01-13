#pragma once

#include "cri_movable.h"

class CRIGameObject : public CRIMovable
{
public:	
    CRIGameObject(const SizeT& Size, const PosT& StartPos);
    CRIGameObject(const SizeT& Size, const PosT& StartPos, const VelT& Velocity);

    void Draw();
    void Update(float Dt);

    void Destroy();
    bool IsDying() const;
    bool IsDead() const;

private:
    virtual void OnDestroyed();

    bool m_IsDead;
};
