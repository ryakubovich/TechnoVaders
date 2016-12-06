#pragma once

#include "alien.hpp"
#include <list>
#include <functional>

using TAliens = std::list<Alien>;

class AI
{
public:
  AI(int aliensNumber, float health, std::string const & name, int holderAmmo,
     float bulletCaliber, float bulletVelocity, BulletManager & bm)
  {
    // Some algo to create aliens and to distribute them across the space
    // This is a test constructor
    for (auto i = 0; i < 3; ++i)
      m_aliens.emplace_back(Alien(Box2D(Point2D(i * 129.0f + 3.0f, 400.0f), Point2D((i+1) * 129.0f + 2.0f, 528.0f)),
                                  100.0f, "M16A1", 10, 1.0f, 50.0f, 0.0f, 0.0f, 0.0f, bm));
  }

  void Update()
  {
    if (m_aliens.size() == 0 && m_noAliensHandler != nullptr) m_noAliensHandler();
  }

  void Shot(); // Some algo to shot player
  TAliens & GetAliens() { return m_aliens; } // Ref is necessary to be non-constant in order to be able to call ait->Damage(damage)
  TAliens::iterator Damage(TAliens::iterator ait, float damage)
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
  using TOnNoAliensHandler = std::function<void()>;
  void SetDamageHandler(TOnDamageHandler const & handler) { m_damageHandler = handler; }
  void SetKillHandler(TOnKillHandler const & handler) { m_killHandler = handler; }
  void SetNoAliensHandler(TOnNoAliensHandler const & handler) { m_noAliensHandler = handler; }

  friend std::ostream & operator << (std::ostream & os, AI const & ai)
  {
    os << "AI: { Aliens: [" << std::endl;
    for (auto const & alien: ai.m_aliens)
      os << alien << "," << std::endl;
    os << "]" << std::endl;
    return os;
  }

private:
  TAliens m_aliens;
  TOnDamageHandler m_damageHandler = nullptr;
  TOnKillHandler m_killHandler = nullptr;
  TOnNoAliensHandler m_noAliensHandler = nullptr;
};
