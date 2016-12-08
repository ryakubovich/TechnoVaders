#pragma once

#include "alien.hpp"
#include <string>

class Player : public Alien
{
public:
  Player(Box2D const & box, float health, int lives, std::string const & name, int holderAmmo,
          float bulletCaliber, float bulletVelocity, float missileCaliber,
          float missileVelocity, float limit, BulletManager & bm, int screenWidth, int screenHeight)
     : Alien(box, health, name, holderAmmo, bulletCaliber, bulletVelocity, missileCaliber, missileVelocity, limit, bm), m_lives(lives),
       m_screenWidth(screenWidth), m_screenHeight(screenHeight)
  {}

  Player(Box2D const & box, float health, int lives, std::string const & name, int holderAmmo,
         float bulletCaliber, float bulletVelocity, float missileCaliber,
         float missileVelocity, float limit, BulletManager & bm)
    : Player(box, health, lives, name, holderAmmo, bulletCaliber, bulletVelocity,
             missileCaliber, missileVelocity, limit, bm, 10000, 5000)
  {}

  void Hit(float damage) { m_gun.AccumulateScore(damage); }
  void Damage(float damage)
  {
    Alien::Damage(damage);
    if (m_health <= 0) DecLives();
  }

  void Resized(int width, int height)
  {
    m_screenWidth = width;
    m_screenHeight = height;
  }

  void Move(Point2D const & offset)
  {
    GameEntity::Move(offset);
    if (m_box.GetMin().x() < 0) GameEntity::Move(Point2D(-m_box.GetMin().x(), 0.0f));
    else if (m_box.GetMax().x() > m_screenWidth)
    {
      Logger::Instance() << *this;
      GameEntity::Move(Point2D(m_screenWidth - m_box.GetMax().x(), 0.0f));
    }
  }

  void Shot() { m_gun.Shot(true, *this); }
  void IncScore() { m_score++; }
  int const & GetScore() const { return m_score; }
  void LaunchMissile()
  {
    if (m_gun.GetScore() >= m_gun.GetLimit()) m_gun.Launch(*this);
    else
    {
      // Do something to highlight lack of score
    }
  }

  using TOnNoLivesHandler = std::function<void()>;
  void SetNoLivesHandler(TOnNoLivesHandler const & handler) { m_noLivesHandler = handler; }

  friend std::ostream & operator << (std::ostream & os, Player const & player)
  {
    os << "Player: { Box = " << player.m_box << " ; Health = " << player.m_health << " ; Gun = " << player.m_gun <<
          " ; Lives = " << player.m_lives << " ; Score = " << player.m_score << " }";
    return os;
  }

private:
  int m_score = 0;
  int m_lives;
  int m_screenWidth;
  int m_screenHeight;
  TOnNoLivesHandler m_noLivesHandler;

  void DecLives()
  {
      m_lives--;
      if (m_lives < 0) m_noLivesHandler();
  }
};
