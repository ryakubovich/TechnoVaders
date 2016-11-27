#pragma once

#include "alien.hpp"
#include <string>

class Player : public Alien
{
public:
  Player(Box2D box, float health, int lives, std::string name, int holderAmmo, float bulletCaliber, float bulletVelocity,
         float missileCaliber, float missileVelocity, float limit, BulletManager & bm)
    : Alien(box, health, name, holderAmmo, bulletCaliber, bulletVelocity, missileCaliber, missileVelocity, limit, bm), m_lives(lives)
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
  int m_lives;
};
