#pragma once

#include "alien.hpp"
#include "player.hpp"
#include "ray2d.hpp"
#include <chrono>
#include <cstdlib>
#include <iterator>
#include <list>
#include <functional>

using TAliens = std::list<Alien>;

class AI
{
public:
  AI(int aliensNumber, float health, std::string const & name, int holderAmmo,
     float bulletCaliber, float bulletVelocity, BulletManager & bm, float shotChance, int screenWidth, int screenHeight)
    : m_screenWidth(screenWidth), m_screenHeight(screenHeight), /*m_kWidth(screenWidth / 1366.0f), m_kHeight(screenHeight / 768.0f),*/
      m_lastShotTimestamp(std::chrono::steady_clock::now()), m_shotChance(shotChance)
  {
    int constructedAliens, i;
    constructedAliens = i = 0;
    int row = 1;
    float x, y;
    while (constructedAliens < aliensNumber)
    {
      x = i * 105.0f + 3.0f;
      y = screenHeight - 50 - 100 * row++;
      m_aliens.emplace_back(Alien(Box2D(Point2D(x, y), Point2D(x + 107.0f, y + 77.0f)), health, name, holderAmmo, bulletCaliber,
                                  bulletVelocity, bm));
      i += row == 3 ? 1 : 0;
      row = row == 3 ? 1 : row;
      constructedAliens++;
    }
  }

  AI(int aliensNumber, float health, std::string const & name, int holderAmmo,
     float bulletCaliber, float bulletVelocity, BulletManager & bm)
    : AI(aliensNumber, health, name, holderAmmo, bulletCaliber, bulletVelocity, bm, 10, 10000, 5000)
  {}

  void Update(float elapsedSeconds)
  {
    bool toMoveDown = false;
    if (m_aliens.begin()->GetBox().GetMin().x() <= 0.0f) m_movement.x() = 100.0f;
    else if (m_aliens.rbegin()->GetBox().GetMax().x() >= m_screenWidth)
    {
      m_movement.x() = -100.0f;
      toMoveDown = true;
    }
    else if (m_movement.y() != 0.0f)
    {
      m_movement.y() = 0.0f;
      toMoveDown = false;
    }
    for (auto it = m_aliens.begin(); it != m_aliens.end(); ++it)
      it->Move(m_movement * elapsedSeconds + Point2D(0.0f, toMoveDown ? -50.0f : 0.0f));
    if (m_aliens.size() == 0 && m_noAliensHandler != nullptr) m_noAliensHandler();
  }

  void Shot(Player const & player)
  {
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_lastShotTimestamp);
    if (duration.count() > 300)
    {
      int alienToShot = rand() % m_aliens.size();
      auto it = m_aliens.begin();
      std::advance(it, alienToShot);
      Ray2D ray = { it->GetBox().GetCenter(), Point2D(0.0f, -1.0f) };
      if (ray.IsRayIntersectingBox(player.GetBox()) && (rand() % 100 <= static_cast<int>(m_shotChance)))
      {
        it->Shot();
        m_lastShotTimestamp = std::chrono::steady_clock::now();
      }
    }
  }

  void Resized(int width, int height)
  {
    m_screenWidth = width;
    m_screenHeight = height;
//    m_kWidth = (width / 1366.0f)  / m_kWidth;
//    m_kHeight = (height / 768.0f) / m_kHeight;
//    for (auto ait = m_aliens.begin(); ait != m_aliens.end(); ++ait)
//    {
//      ait->ResizeBox(m_kWidth, m_kHeight);
//      ait->Move(Point2D(0.0f, height - ait->GetBox().GetMax().y()));
//    }
//    Logger::Instance() << m_kWidth << m_kHeight;
  }


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
  int m_screenWidth;
  int m_screenHeight;
//  float m_kWidth;
//  float m_kHeight;
  Point2D m_movement = { 100.0f, 0.0f };
  std::chrono::time_point<std::chrono::steady_clock> m_lastShotTimestamp;
  float m_shotChance;
  TAliens m_aliens;
  TOnDamageHandler m_damageHandler = nullptr;
  TOnKillHandler m_killHandler = nullptr;
  TOnNoAliensHandler m_noAliensHandler = nullptr;
};
