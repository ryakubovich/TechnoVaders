#pragma once

#include "gameentity.hpp"
#include <list>

class Bullet : GameEntity
{
public:
  Bullet(box)
    : GameEntity(box)
  {}

  Bullet(box, velocity, power, direction)
    : GameEntity(box), m_velocity(velocity), m_power(power), m_direction(direction)
  {}

  void Move() { GameEntity::Move(m_direction * m_velocity); }

private:
  float m_velocity = 1;
  float m_power = 100;
  Point2D m_direction = { 0.0f, 1.0f };
};

class Missile : Bullet
{
public:
  Missile(box, velocity, power, direction)
    : Bullet(box, velocity, power, direction)
  {}

  void MoveLeft() { GameEntity::Move(Point2D(-1.0f, 0.0f)); }
  void MoveRight() { GameEntity::Move(Point2D(1.0f, 0.0f)); }
};

class AmmoManager
{
public:
  void Update()
  {
    for (auto it = m_playersBullets.begin(); it != m_playersBullets.end(); ++it)
      it->Move();
  }

  void CreateBullet(Box2D box)
  {
    m_playersBullets.emplace_back(Bullet(box));
  }

  void CreateBullet(Box2D box, float velocity, float power, Point2D direction)
  {
    m_playersBullets.emplace_back(Bullet(box, velocity, power, direction));
  }

  void DeleteBullet(bool playersBullet, std::list<Bullet>::iterator it)
  {
    if (playersBullet)
      m_playersBullets.erase(it);
    else
      m_aliensBullets.erase(it);
  }

  void CreateMissile();
  void DeleteMissile();

  std::list<Bullet> const & GetPlayersBullets() const { return m_playersBullets; }
  std::list<Bullet> const & GetAliensBullets() const { return m_aliensBullets; }
  std::list<Missile> const & GetPlayersMissiles() const { return m_playersMissiles; }

private:
  std::list<Bullet> m_playersBullets;
  std::list<Bullet> m_aliensBullets;
  std::list<Missile> m_playersMissiles;
};
