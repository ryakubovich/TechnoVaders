#pragma once

#include "gameentity.hpp"
#include "gun.hpp"
#include <string>

class Alien : public GameEntity
{
public:
  Alien(Box2D box, float health, std::string name, int holderAmmo, float bulletCaliber, float bulletVelocity,
        float missileCaliber, float missileVelocity, float limit, BulletManager & bm)
    : GameEntity(box), m_health(health), m_gun(name, holderAmmo, bulletCaliber, bulletVelocity,
                                               missileCaliber, missileVelocity, limit, bm) {}

  Alien(Box2D box, float health, std::string name, int holderAmmo, float caliber, float velocity, BulletManager & bm)
    : Alien(box, health, name, holderAmmo, caliber, velocity, 0.0f, 0.0f, 0.0f, bm) {}

  void Shot() { m_gun.Shot(this); }
  void Damage(float damage) { m_health -= damage; }
  float GetHealth() const { return m_health; }
  Gun const & GetGun() const { return m_gun; }
  
protected:
  float m_health = 100;
  Gun m_gun;
};

  
