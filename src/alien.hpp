#pragma once

#include "gameentity.hpp"
#include "gun.hpp"

class Alien : public GameEntity
{
public:
  Alien(Box2D box, Gun gun)
    : GameEntity(box), m_gun(gun)
  {}

  Alien(Box2D box, float health, Gun gun)
    : GameEntity(box), m_health(health), m_gun(gun)
  {}

  void Shot() { m_gun.Shot(this); }

  void Damage(float damage)
  {
    m_health -= damage;
  }
  float GetHealth() const { return m_health; }
  
protected:
  float m_health = 100;
  Gun m_gun;
};

  
