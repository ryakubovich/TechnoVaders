#pragma once

#include "alien.hpp"

class Player : public Alien
{
public:
  Player(Box2D box, float health, Gun gun)
    : Alien(box, health, gun)
  {}

  Player(Box2D box, float health, Gun gun, int lives)
    : Alien(box, health, gun), m_lives(lives)
  {}

  void Hit(float damage)
  {
    m_health -= damage;
    if (m_health < 0.0)
    {
      DecLives();
    }
  }

  void DecLives() { m_lives--; }
  void LaunchMissile()
  {
    CreateMissile();
    m_limit = 0;
  }

private:
  int m_lives = 3;
};
