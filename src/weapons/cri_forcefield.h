#pragma once

#include "cri_game_object.h"

#include <vector>

class CRIEnemy;
class CRIPlayer;

class CRIForcefield : public CRIGameObject
{
public:
    CRIForcefield(const float Radius, const PosT& Pos, int Time, int Damage,
        float PushForce, const CRIPlayer& Player);

    bool Affect(const CRIEnemy& Enemy);
    int GetDamage() const;

    void LogicUpdate(float Dt); // override
    ci::Vec2f GetPushVector(ci::Vec2f ToPos) const;

    static int TextureDescr;

private:
    typedef std::vector<const CRIEnemy*> AffectedContT;
    typedef AffectedContT::iterator AffectedIterT;

    void DoDraw(); // override

    float m_TimeLeft;
    float m_EffectTime;
    int m_Damage;
    float m_PushForce;
    float m_TargetRadius;

    AffectedContT m_Affected;
    const CRIPlayer* m_pPlayer;
};
