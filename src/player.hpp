#pragma once

#include "alien.hpp"
#include "missilelauncher.hpp"

class Player : Alien
{
public:
  Player(health, gun)
    : Alien(health, gun)
  {}

  Player(health, gun, lives)
    : Alien(health, gun), m_lives(lives)
  {}

   void MoveLeft() { GameEntity::Move(Point2D(-1.0f, 0.0f)); }
   void MoveRight() { GameEntity::Move(Point2D(1.0f, 0.0f)); }

private:
  int m_lives = 3;
};
