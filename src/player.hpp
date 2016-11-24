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

  void DecLives() { m_lives--; }

private:
  int m_lives = 3;
};
