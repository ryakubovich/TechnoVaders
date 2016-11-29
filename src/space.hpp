#pragma once

#include <functional>
#include <fstream>
#include <list>
#include "ai.hpp"
#include "ammo.hpp"
#include "obstacle.hpp"
#include "player.hpp"

enum InputType
{
  Shot,
  LaunchMissile,
  MoveLeft,
  MoveRight,
  MoveMissileLeft,
  MoveMissileRight
};

class EndOfTheGameException : public std::exception {};

class Space
{
public:
  // TO DO: constructor from std::map
  Space(float pHealth, int pLives, std::string pGunName, int pGunHolderAmmo, float pGunBulletCaliber,
        float pGunBulletVelocity, float pGunMissileCaliber, float pGunMissileVelocity, float pGunLimit,
        int aNumber, float aHealth, std::string aGunName, int aGunHolderAmmo, float aGunBulletCaliber,
        float aGunBulletVelocity)
    : m_playerOne(Box2D(), pHealth, pLives, pGunName, pGunHolderAmmo, pGunBulletCaliber, pGunBulletVelocity,
                  pGunMissileCaliber, pGunMissileVelocity, pGunLimit, m_bm),
      m_ai(aNumber, aHealth, aGunName, aGunHolderAmmo, aGunBulletCaliber, aGunBulletVelocity, m_bm)
  {
    m_ai.SetDamageHandler([this](float damage) { m_playerOne.Hit(damage); });
    m_ai.SetKillHandler([this]() { m_playerOne.IncScore(); });
    m_playerOne.SetNoLivesHandler([]() { throw EndOfTheGameException(); }); // Exception is caught in GameManager
  }

  void Update()
  {
    m_bm.Update();
    m_ai.Update();
    IntersectionCheck();
    m_ai.Shot();
  }

  void InputProcessing(InputType input);

  friend std::ostream & operator << (std::ostream & os, Space const & space)
  {
    os << "Space: {";
    os << space.m_bm;
    os << space.m_ai;
    os << space.m_playerOne;
    os << "Obstacles: [";
    for (auto const & obstacle: space.m_obstacles)
      os << obstacle;
    return os;
  }

private:
  BulletManager m_bm;
  AI m_ai;
  Player m_playerOne;
//  Player m_playerTwo;
  std::list<Obstacle> m_obstacles;

  void IntersectionCheck()
  {
    auto playersBullets = m_bm.GetPlayersBullets();
    auto aliens = m_ai.GetAliens();
    for (auto pit = playersBullets.begin(); pit != playersBullets.end(); ++pit)
      for (auto ait = aliens.begin(); ait != aliens.end(); ++ait)
      {
        if (pit->GetBox().IsBoxIntersectingBox(ait->GetBox()))
        {
          m_ai.Damage(ait, pit->GetPower());
          m_bm.DeleteBullet(true, pit);
          break;
        }
      }
    auto aliensBullets = m_bm.GetAliensBullets();
    for (auto ait = aliensBullets.begin(); ait != aliensBullets.end(); ++ait)
    {
      if ((ait->GetBox()).IsBoxIntersectingBox(m_playerOne.GetBox()))
      {
        m_playerOne.Damage(ait->GetPower());
        m_bm.DeleteBullet(false, ait);
        break;
      }
      for (auto oit = m_obstacles.begin(); oit != m_obstacles.end(); ++oit)
      {
        if (ait->GetBox().IsBoxIntersectingBox(oit->GetOverallBox()))
        {
          oit->Damage(ait->GetBox());
          m_bm.DeleteBullet(false, ait);
          break;
        }
      }
    }
  }
};
