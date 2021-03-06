#pragma once

class HealthMixin {
  public:
    virtual ~HealthMixin() {}

    void SetMaxHealth(int Max);
    void ModifyHealth(int Val);
    void ForceSetHealthValue(int Val);

    int GetCurHealthValue() const;
    int GetMaxHealthValue() const;

  private:
    virtual void OnHealthDepleted() = 0;
    virtual void OnHealthModified(int NewVal, int Modifier);

    int m_CurVal{};
    int m_MaxVal{};
};

bool HasFullHealth(const HealthMixin& Mixin);
