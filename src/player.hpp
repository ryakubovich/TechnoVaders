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
    m_gun.AccumulateScore(damage);
  }

  void DecLives() { m_lives--; }
  void IncScore() { m_score++; }
  void LaunchMissile()
  {
    if (m_gun.GetScore() >= m_gun.GetLimit()) m_gun.Launch(this);
    else
    {
      // Do something to highlight lack of score
    }
  }

private:
  int m_score = 0;
  int m_lives = 3;
};
