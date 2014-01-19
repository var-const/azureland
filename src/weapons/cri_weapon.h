#pragma once

namespace cinder { template <typename T> class Vec2; }

class CRIGameScene;
class CRIWeapon;

class CRIWeapon
{
public:	
    CRIWeapon();
    virtual ~CRIWeapon();

    void Shoot(cinder::Vec2<float> StartPos, cinder::Vec2<float> TargetPos);

    void SetScene(CRIGameScene& Scene);

protected:
    CRIGameScene& GetScene();
    const CRIGameScene& GetScene() const;

private:
    virtual void DoShoot(cinder::Vec2<float> StartPos,
        cinder::Vec2<float> TargetPos) = 0;

    CRIGameScene* m_pScene; // Non-owning
};
