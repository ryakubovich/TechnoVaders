#pragma once

#include <functional>
#include <fstream>
#include <list>
#include "ai.hpp"
#include "ammo.hpp"
#include "obstacle.hpp"
#include "player.hpp"

using TObstacles = std::list<Obstacle>;
enum InputType
{
  Shot,
  LaunchMissile,
  MoveLeft,
  MoveRight,
  MoveMissileLeft,
  MoveMissileRight
};

enum WinnerType
{
  PlayerWinner,
  AIWinner
};

class EndOfTheGameException : public std::exception
{
public:
  EndOfTheGameException(WinnerType winner)
    : m_winner(winner)
  {}
  WinnerType GetWinner() const { return m_winner; }
private:
  WinnerType m_winner;
};

class WrongInputException : public std::exception {};

class Space
{
public:
  // TO DO: constructor from std::map; change player's box
  Space(float pHealth, int pLives, std::string const & pGunName, int pGunHolderAmmo,
        float const & pGunBulletCaliber, float const & pGunBulletVelocity, float pGunMissileCaliber,
        float pGunMissileVelocity, float pGunLimit, int aNumber, float aHealth,
        std::string const & aGunName, int aGunHolderAmmo, float aGunBulletCaliber,
        float aGunBulletVelocity)
    : m_playerOne(Box2D(20.0f, 20.0f, 110.0f, 164.0f), pHealth, pLives, pGunName, pGunHolderAmmo, pGunBulletCaliber, pGunBulletVelocity,
                  pGunMissileCaliber, pGunMissileVelocity, pGunLimit, m_bm),
      m_ai(aNumber, aHealth, aGunName, aGunHolderAmmo, aGunBulletCaliber, aGunBulletVelocity, m_bm)
  {
    m_ai.SetDamageHandler([this](float damage, float health) { m_playerOne.Hit(damage > health ? health : damage); });
    m_ai.SetKillHandler([this]() { m_playerOne.IncScore(); });
    m_playerOne.SetNoLivesHandler([]() { throw EndOfTheGameException(WinnerType::AIWinner); }); // Exception is caught in GameManager
    m_ai.SetNoAliensHandler([]() { throw EndOfTheGameException(WinnerType::PlayerWinner); });

    for (int i = 0; i < 5; ++i)
    {
      m_obstacles.emplace_back(Obstacle(Point2D(i * 50.0f + 70.0f, 170.0f), Point2D((i+1) * 50.0f + 40.0f, 270.0f), 2.0f, 2.0f));
    }
  }

  void Update(float elapsedSeconds)
  {
    m_bm.Update(elapsedSeconds);
    m_ai.Update(elapsedSeconds);
    IntersectionCheck();
    //m_ai.Shot();
  }

  void InputProcessing(InputType input, float elapsedSeconds = 0.0f)
  {
    switch(input)
    {
      case InputType::Shot:
        m_playerOne.Shot();
        break;
      case InputType::MoveLeft:
        m_playerOne.Move(Point2D(-300.0f, 0.0f) * elapsedSeconds);
        break;
      case InputType::MoveRight:
        m_playerOne.Move(Point2D(300.0f, 0.0f) * elapsedSeconds);
        break;
      case InputType::LaunchMissile:
        m_playerOne.LaunchMissile();
        break;
      case InputType::MoveMissileLeft:
        m_bm.MoveMissile(elapsedSeconds, Point2D(-1.0f, 0.0f));
        break;
      case InputType::MoveMissileRight:
        m_bm.MoveMissile(elapsedSeconds, Point2D(1.0f, 0.0f));
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
  TObstacles const & GetObstacles() const { return m_obstacles; }

private:
  BulletManager m_bm;
  AI m_ai;
  Player m_playerOne;
//  Player m_playerTwo;
  TObstacles m_obstacles;

  void IntersectionCheck()
  {
    // TO DO: delete players' bullets when hitting obstacles; combine missiles and players' bullets check to avoid double-cycling aliens
    TBullets bulletsToRemove;
//    TAliens aliensToRemove;
    for (auto pit = m_bm.GetPlayersBullets().begin(); pit != m_bm.GetPlayersBullets().end(); ++pit)
    {
      for (auto ait = m_ai.GetAliens().begin(); ait != m_ai.GetAliens().end(); ++ait)
        if (pit->GetBox().IsBoxIntersectingBox(ait->GetBox()))
        {
          ait = m_ai.Damage(ait, pit->GetPower());
//          aliensToRemove.push_back(*ait);
          bulletsToRemove.push_back(*pit);
          break;
        }
//      for (auto const & alien : aliensToRemove)
//        m_ai.RemoveAlien(alien);
//      aliensToRemove.clear();
    }

    for (auto const & bullet : bulletsToRemove) { m_bm.DeleteBullet(true, bullet); }
    bulletsToRemove.clear();

    for (auto mit = m_bm.GetPlayersMissiles().begin(); mit != m_bm.GetPlayersMissiles().end(); ++mit)
    {
      for (auto ait = m_ai.GetAliens().begin(); ait != m_ai.GetAliens().end(); ++ait)
        if (ait->GetBox().IsBoxIntersectingBox(mit->GetBox()))
        {
          ait = m_ai.Damage(ait, mit->GetPower());
//          aliensToRemove.push_back(*ait);
          Logger::Instance() << "Alien deleted";
          bulletsToRemove.push_back(*mit);
          break;
        }
//      for (auto const & alien : aliensToRemove)
//        m_ai.RemoveAlien(alien);
//      aliensToRemove.clear();
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
