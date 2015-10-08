#pragma once

namespace cinder { template <typename T> class Vec2; }

class CRIGameScene;

class CRIPickupGenerator
{
public:	
    CRIPickupGenerator();

    void Invoke(cinder::Vec2<float> PosForPickUp);

    void SetScene(CRIGameScene& Scene);

private:
    CRIGameScene* m_pScene; // Non-owning
};
