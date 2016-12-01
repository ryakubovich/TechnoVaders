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
    // This is a test constructor
    for (auto i = 0; i < 3; ++i)
      m_aliens.emplace_back(Alien(Box2D(Point2D(i * 4.0f + 3.0f, 10.0f), Point2D((i+1) * 4.0f + 2.0f, 12.0f)),
                                  100.0f, "M16A1", 10, 1.0f, 50.0f, 0.0f, 0.0f, 0.0f, bm));
  }

  void Update(); // Update positions of aliens
  void Shot(); // Some algo to shot player
  std::list<Alien> & GetAliens() { return m_aliens; }
  std::list<Alien>::iterator Damage(std::list<Alien>::iterator ait, float const & damage)
  {
    if (m_damageHandler != nullptr) m_damageHandler(damage, ait->GetHealth());
    if (ait->GetHealth() > damage)
    {
      ait->Damage(damage);
      return ait;
    }
    else
    {
      if (m_killHandler != nullptr) m_killHandler();
      return m_aliens.erase(ait);
    }
  }

  using TOnDamageHandler = std::function<void(float, float)>;
  using TOnKillHandler = std::function<void()>;
  void SetDamageHandler(TOnDamageHandler const & handler) { m_damageHandler = handler; }
  void SetKillHandler(TOnKillHandler const & handler) { m_killHandler = handler; }

  friend std::ostream & operator << (std::ostream & os, AI ai)
  {
    os << "AI: { Aliens: [\n";
    for (auto const & alien: ai.m_aliens)
      os << alien << "," << std::endl;
    os << "]" << std::endl;
    return os;
  }

private:
  std::list<Alien> m_aliens;
  TOnDamageHandler m_damageHandler = nullptr;
  TOnKillHandler m_killHandler = nullptr;
};
