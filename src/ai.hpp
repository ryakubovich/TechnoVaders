#pragma once

#include "alien.hpp"
#include <list>
#include <functional>

class AI
{
public:
  void Update();
  void Shot();
  std::list<Alien> const & GetAliens() const { return m_aliens; }
  void Damage(std::list<Alien>::iterator ait, float damage)
  {
    if (m_damageHandler != nullptr) m_damageHandler(damage);
    if (ait->GetHealth() > damage) ait->Damage(damage);
    else
    {
      m_aliens.erase(ait);
      if (m_killHandler != nullptr) m_killHandler();
    }
  }

  using TOnKillHandler = std::function<void()>;
  using TOnDamageHandler = std::function<void(float)>;
  void SetDamageHandler(TOnDamageHandler const & handler) { m_damageHandler = handler; }
  void SetKillHandler(TOnKillHandler const & handler) { m_killHandler = handler; }

private:
  std::list<Alien> m_aliens;
  TOnDamageHandler m_damageHandler = nullptr;
  TOnKillHandler m_killHandler = nullptr;
};
