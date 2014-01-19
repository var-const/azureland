#pragma once

#include "cri_game_object.h"

#include <vector>

class CRIEnemy;

class CRIForcefield : public CRIGameObject
{
public:
    CRIForcefield(const float Radius, const PosT& Pos, int Time, int Damage,
        float PushForce, float PushDecceleration);

    bool Affect(const CRIEnemy& Enemy);
    int GetDamage() const;

    void LogicUpdate(float Dt); // override

private:
    typedef std::vector<const CRIEnemy*> AffectedContT;
    typedef AffectedContT::iterator AffectedIterT;

    void DoDraw(); // override

    float m_TimeLeft;
    float m_EffectTime;
    int m_Damage;
    float m_PushForce;
    float m_PushDecceleration;
    float m_TargetRadius;

    AffectedContT m_Affected;
};
