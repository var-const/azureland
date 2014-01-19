#pragma once

class CRIHealthMixin
{
public:
    CRIHealthMixin();
    virtual ~CRIHealthMixin();

    void SetMax(int Max);
    void Modify(int Val);
    void ForceSetValue(int Val);

    int GetCurValue() const;
    int GetMaxValue() const;

private:
    virtual void Die() = 0;

    int m_CurVal;
    int m_MaxVal;
};
