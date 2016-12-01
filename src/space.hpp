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
class WrongInputException : public std::exception {};

class Space
{
public:
  // TO DO: constructor from std::map
  Space(float pHealth, int pLives, std::string pGunName, int pGunHolderAmmo, float pGunBulletCaliber,
        float pGunBulletVelocity, float pGunMissileCaliber, float pGunMissileVelocity, float pGunLimit,
        int aNumber, float aHealth, std::string aGunName, int aGunHolderAmmo, float aGunBulletCaliber,
        float aGunBulletVelocity)
    : m_playerOne(Box2D(1.0f, 1.0f, 6.0f, 6.0f), pHealth, pLives, pGunName, pGunHolderAmmo, pGunBulletCaliber, pGunBulletVelocity,
                  pGunMissileCaliber, pGunMissileVelocity, pGunLimit, m_bm),
      m_ai(aNumber, aHealth, aGunName, aGunHolderAmmo, aGunBulletCaliber, aGunBulletVelocity, m_bm)
  {
    m_ai.SetDamageHandler([this](float damage, float health) { m_playerOne.Hit(damage > health ? health : damage); });
    m_ai.SetKillHandler([this]() { m_playerOne.IncScore(); });
    m_playerOne.SetNoLivesHandler([]() { throw EndOfTheGameException(); }); // Exception is caught in GameManager
  }

  void Update()
  {
    m_bm.Update();
    //m_ai.Update();
    IntersectionCheck();
    //m_ai.Shot();
  }

  // TO DO: calculate movements to depend on movement in a second and elapsed milliseconds
  void InputProcessing(InputType input)
  {
    switch(input)
    {
      case InputType::Shot:
        m_playerOne.Shot();
        break;
      case InputType::MoveLeft:
        m_playerOne.Move(Point2D(-1.0f, 0.0f));
        break;
      case InputType::MoveRight:
        m_playerOne.Move(Point2D(1.0f, 0.0f));
        break;
      case InputType::LaunchMissile:
        m_playerOne.LaunchMissile();
        break;
      case InputType::MoveMissileLeft:
        m_bm.MoveMissile(Point2D(-1.0f, 0.0f));
        break;
      case InputType::MoveMissileRight:
        m_bm.MoveMissile(Point2D(1.0f, 0.0f));
        break;
      default:
        throw WrongInputException();
        break;
    }
  }

  friend std::ostream & operator << (std::ostream & os, Space const & space)
  {
    os << "Space: {";
    os << space.m_bm;
    os << space.m_ai;
    os << space.m_playerOne;
    os << "Obstacles: [";
    for (auto const & obstacle: space.m_obstacles)
      os << obstacle << "," << std::endl;
    return os;
  }

  // TO DO: add method to return all drawable objects in space
  // These methods are only for testing purposes:
  Player const & GetPlayer() const { return m_playerOne; }
  BulletManager const & GetBM() const { return m_bm; }
  AI & GetAI() { return m_ai; }
  std::list<Obstacle> const & GetObstacles() const { return m_obstacles; }

private:
  BulletManager m_bm;
  AI m_ai;
  Player m_playerOne;
//  Player m_playerTwo;
  std::list<Obstacle> m_obstacles;

  void IntersectionCheck()
  {
    // TO DO: delete players' bullets when hitting obstacles; combine missiles and players' bullets check to avoid double-cycling aliens
    std::list<Bullet> bulletsToRemove;
    for (auto pit = m_bm.GetPlayersBullets().begin(); pit != m_bm.GetPlayersBullets().end(); ++pit)
      for (auto ait = m_ai.GetAliens().begin(); ait != m_ai.GetAliens().end(); ++ait)
        if (pit->GetBox().IsBoxIntersectingBox(ait->GetBox()))
        {
          ait = m_ai.Damage(ait, pit->GetPower());
          bulletsToRemove.push_back(*pit);
          break;
        }

    for (auto const & bullet : bulletsToRemove) { m_bm.DeleteBullet(true, bullet); }
    bulletsToRemove.clear();

    for (auto mit = m_bm.GetPlayersMissiles().begin(); mit != m_bm.GetPlayersMissiles().end(); ++mit)
      for (auto ait = m_ai.GetAliens().begin(); ait != m_ai.GetAliens().end(); ++ait)
        if (mit->GetBox().IsBoxIntersectingBox(ait->GetBox()))
        {
          ait = m_ai.Damage(ait, mit->GetPower());
          bulletsToRemove.push_back(*mit);
          break;
        }

    for (auto const & missile : bulletsToRemove) { m_bm.DeleteMissile(missile); }
    bulletsToRemove.clear();

    for (auto ait = m_bm.GetAliensBullets().begin(); ait != m_bm.GetAliensBullets().end(); ++ait)
    {
      if ((ait->GetBox()).IsBoxIntersectingBox(m_playerOne.GetBox()))
      {
        m_playerOne.Damage(ait->GetPower());
        bulletsToRemove.push_back(*ait);
        break;
      }
      for (auto oit = m_obstacles.begin(); oit != m_obstacles.end(); ++oit)
        if (ait->GetBox().IsBoxIntersectingBox(oit->GetOverallBox()))
        {
          if (oit->Damage(ait->GetBox())) oit = m_obstacles.erase(oit);
          bulletsToRemove.push_back(*ait);
          break;
        }
    }
    for (auto const & bullet : bulletsToRemove) { m_bm.DeleteBullet(false, bullet); }
  }
};
