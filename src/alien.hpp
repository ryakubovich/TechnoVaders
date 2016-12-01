#pragma once

#include "gameentity.hpp"
#include "gun.hpp"
#include <string>

class Alien : public GameEntity
{
public:
  // Constructor for calling from Player instances (controlled from AI not to be called)
  Alien(Box2D const & box, float const & health, std::string const & name, int const & holderAmmo,
        float const & bulletCaliber, float const & bulletVelocity, float const & missileCaliber,
        float const & missileVelocity, float const & limit, BulletManager & bm)
    : GameEntity(box), m_health(health), m_gun(name, holderAmmo, bulletCaliber, bulletVelocity,
                                               missileCaliber, missileVelocity, limit, bm)
  {}

  Alien(Box2D const & box, float const & health, std::string const & name, int const & holderAmmo,
        float const & caliber, float const & velocity, BulletManager & bm)
    : Alien(box, health, name, holderAmmo, caliber, velocity, 0.0f, 0.0f, 0.0f, bm)
  {}

  void Shot() { m_gun.Shot(0, *this); }
  void Damage(float const & damage) { m_health -= damage; }
  float GetHealth() const { return m_health; }
  Gun const & GetGun() const { return m_gun; }

  friend std::ostream & operator << (std::ostream & os, Alien const & alien)
  {
    os << "Alien: { Box = " << alien.m_box << " ; Health = " << alien.m_health << " ; Gun = " << alien.m_gun << " }";
    return os;
  }
  
protected:
  float m_health = 100;
  Gun m_gun;
};

  
