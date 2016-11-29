#pragma once

#include "alien.hpp"
#include <list>
#include <functional>

class AI
{
public:
  AI(int aliensNumber, float health, std::string name, int holderAmmo, float bulletCaliber, float bullerVelocity, BulletManager & bm)
  {
    // Some algo to create aliens and to distibute them across the space
    for (auto i = 0; i != 3; ++i)
      m_aliens.emplace_back(Alien(Box2D(Point2D(i * 4.0f + i * 2.0f, 10.0f), Point2D((i+1) * 4.0f + i * 2.0f, 12.0f)),
                                  100.0f, "M16A1", 10, 1.0f, 50.0f, 0.0f, 0.0f, 0.0f, bm));
  }

  void Update(); // Update positions of aliens
  void Shot(); // Some algo to shot player
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

  using TOnDamageHandler = std::function<void(float)>;
  using TOnKillHandler = std::function<void()>;
  void SetDamageHandler(TOnDamageHandler const & handler) { m_damageHandler = handler; }
  void SetKillHandler(TOnKillHandler const & handler) { m_killHandler = handler; }

private:
  std::list<Alien> m_aliens;
  TOnDamageHandler m_damageHandler = nullptr;
  TOnKillHandler m_killHandler = nullptr;
};
