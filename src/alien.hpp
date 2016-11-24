#pragma once

#include "gameentity.hpp"
#include "gun.hpp"

class Alien : GameEntity
{
public:
  Alien(box)
    : GameEntity(box)
  {}

  Alien(box, health, gun)
    : GameEntity(box), m_health(health), m_gun(gun)
  {}

  void Move() { GameEntity::Move(m_direction * m_velocity); }
  void Shot();
  void Hit();
  
private:
  float m_health = 100;
  Gun m_gun;
};

  
