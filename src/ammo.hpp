#pragma once

#include "gameentity.hpp"
#include <list>

class Bullet : public GameEntity
{
public:
  Bullet(Box2D box)
    : GameEntity(box)
  {}

  Bullet(Box2D box, float velocity, float power, Point2D direction)
    : GameEntity(box), m_velocity(velocity), m_power(power), m_direction(direction)
  {}

  void Move() { GameEntity::Move(m_direction * m_velocity); }
  float GetPower() { return m_power; }

private:
  float m_velocity = 1;
  float m_power = 100;
  Point2D m_direction = { 0.0f, 1.0f };
};

class BulletManager
{
public:
  void Update()
  {
    for (auto it = m_playersBullets.begin(); it != m_playersBullets.end(); ++it)
      it->Move();
  }

  void CreateBullet(Box2D box) { m_playersBullets.emplace_back(Bullet(box)); }
  void CreateBullet(Box2D box, float velocity, float power, Point2D direction) { m_playersBullets.emplace_back(Bullet(box, velocity, power, direction)); }

  void DeleteBullet(bool playersBullet, std::list<Bullet>::iterator it)
  {
    if (playersBullet)
      m_playersBullets.erase(it);
    else
      m_aliensBullets.erase(it);
  }

  void CreateMissile(Box2D box) { m_playersMissiles.emplace_back(Bullet(box)); }
  void CreateMissile(Box2D box, float velocity, float power, Point2D direction) { m_playersMissiles.emplace_back(Bullet(box, velocity, power, direction)); }
  void DeleteMissile(std::list<Bullet>::iterator it) { m_playersMissiles.erase(it); }

  std::list<Bullet> const & GetPlayersBullets() const { return m_playersBullets; }
  std::list<Bullet> const & GetAliensBullets() const { return m_aliensBullets; }
  std::list<Bullet> const & GetPlayersMissiles() const { return m_playersMissiles; }

private:
  // Lists are used because of frequent deletion of randomly placed bullets
  std::list<Bullet> m_playersBullets;
  std::list<Bullet> m_aliensBullets;
  std::list<Bullet> m_playersMissiles;
};
