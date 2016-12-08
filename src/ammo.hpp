#pragma once

#include "gameentity.hpp"
#include <list>

class Bullet : public GameEntity
{
public:
  Bullet(Box2D const & box)
    : GameEntity(box)
  {}

  Bullet(Box2D const & box, float velocity, float power, Point2D const & direction)
    : GameEntity(box), m_velocity(velocity), m_power(power), m_direction(direction)
  {}

  void Move(float elapsedSeconds, Point2D movement = Point2D(0.0f, 0.0f)) { GameEntity::Move(m_direction * m_velocity * elapsedSeconds + movement); }
  float const & GetPower() const { return m_power; }

  bool operator == (Bullet const & bullet)
  {
    return m_box == bullet.m_box
        && m_velocity == bullet.m_velocity
        && m_power == bullet.m_power
        && m_direction == bullet.m_direction;
  }

  friend std::ostream & operator << (std::ostream & os, Bullet const & bullet)
  {
    os << "Bullet: { Box = " << bullet.m_box << " ; Velocity = " << bullet.m_velocity << " ; Power = " << bullet.m_power <<
          " ; Direction = " << bullet.m_direction << " }";
    return os;
  }

private:
  float m_velocity = 1;
  float m_power = 100;
  Point2D m_direction = { 0.0f, 1.0f };
};

using TBullets = std::list<Bullet>;

class BulletManager
{
public:
  void Update(float elapsedSeconds)
  {
    for (auto it = m_playersBullets.begin(); it != m_playersBullets.end(); ++it)
      it->Move(elapsedSeconds);
    for (auto mit = m_playersMissiles.begin(); mit != m_playersMissiles.end(); ++mit)
      mit->Move(elapsedSeconds);
    for (auto ait = m_aliensBullets.begin(); ait != m_aliensBullets.end(); ++ait)
      ait->Move(elapsedSeconds);
  }

  void CreateBullet(bool playersBullet, Box2D const & box)
  {
    if (playersBullet) m_playersBullets.emplace_back(Bullet(box));
    else m_aliensBullets.emplace_back(Bullet(box));
  }
  void CreateBullet(bool playersBullet, Box2D const & box, float velocity, float power, Point2D const & direction)
  {
    if (playersBullet) m_playersBullets.emplace_back(Bullet(box, velocity, power, direction));
    else m_aliensBullets.emplace_back(Bullet(box, velocity, power, direction));
  }

  bool DeleteBullet(bool playersBullet, Bullet const & bullet)
  {
    try
    {
      if (playersBullet) m_playersBullets.remove(bullet);
      else m_aliensBullets.remove(bullet);
      return true;
    }
    catch(...)
    {
      return false;
    }
  }

  void CreateMissile(Box2D const & box) { m_playersMissiles.emplace_back(Bullet(box)); }
  void CreateMissile(Box2D const & box, float velocity, float power, Point2D const & direction)
  {
    m_playersMissiles.emplace_back(Bullet(box, velocity, power, direction));
  }

  bool DeleteMissile(Bullet const & bullet)
  {
    try
    {
      m_playersMissiles.remove(bullet);
      return true;
    }
    catch(...) { return false; }
  }

  bool MoveMissile(float elapsedSeconds, Point2D const & movement)
  {
    try
    {
      auto it = m_playersMissiles.begin();
      it->Move(elapsedSeconds, movement);
    }
    catch (std::exception const & ex)
    {
      Logger::Instance() << ex.what();
      return false;
    }
    return true;
  }

  TBullets const & GetPlayersBullets() const { return m_playersBullets; }
  TBullets const & GetAliensBullets() const { return m_aliensBullets; }
  TBullets const & GetPlayersMissiles() const { return m_playersMissiles; }

  friend std::ostream & operator << (std::ostream & os, BulletManager const & bm)
  {
    os << "BulletManager: { PlayersBullets = [\n";
    for (auto const & pb : bm.m_playersBullets)
      os << pb << "," << std::endl;
    os << "]\nPlayersMissiles = [";
    for (auto const & pm : bm.m_playersMissiles)
      os << pm << "," << std::endl;
    os << "]\nAliensBullets = [";
    for (auto const & ab : bm.m_aliensBullets)
      os << ab << "," << std::endl;
    os << "] }";
    return os;
  }

private:
  // Lists are used because of frequent deletion of randomly placed bullets
  TBullets m_playersBullets;
  TBullets m_aliensBullets;
  TBullets m_playersMissiles;
};
