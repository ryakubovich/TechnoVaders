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
       m_screenWidth(screenWidth), m_screenHeight(screenHeight)/*, m_kWidth(screenWidth / 1366.0f), m_kHeight(screenHeight / 768.0f)*/
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
//    m_kWidth = (width / 1366.0f) / m_kWidth;
//    m_kHeight = (height / 768.0f) / m_kHeight;
//    GameEntity::ResizeBox(m_kWidth, m_kHeight);
//    Logger::Instance() << m_kWidth << m_kHeight;
  }

  void Move(Point2D const & offset)
  {
    GameEntity::Move(offset/*Point2D(m_kWidth * offset.x(), m_kHeight * offset.y())*/);
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
  int const & GetLives() const { return m_lives; }
  bool IsLaunchable() const { return m_gun.GetScore() >= m_gun.GetLimit() && !m_gun.IsLaunched(); }
  void LaunchMissile() { if (IsLaunchable()) m_gun.Launch(*this); }

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
//  float m_kWidth;
//  float m_kHeight;
  TOnNoLivesHandler m_noLivesHandler;

  void DecLives()
  {
      m_lives--;
      if (m_lives < 0) m_noLivesHandler();
      m_health = 100;
  }
};
