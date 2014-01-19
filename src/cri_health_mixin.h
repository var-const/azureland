#pragma once

class CRIHealthMixin
{
public:
    CRIHealthMixin();
    virtual ~CRIHealthMixin();

    void SetMaxHealth(int Max);
    void ModifyHealth(int Val);
    void ForceSetHealthValue(int Val);

    int GetCurHealthValue() const;
    int GetMaxHealthValue() const;

private:
    virtual void OnHealthDepleted() = 0;

    int m_CurVal;
    int m_MaxVal;
};
