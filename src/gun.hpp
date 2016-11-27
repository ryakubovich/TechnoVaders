#pragma once

#include <string>
#include <ctime>
#include "ammo.hpp"
#include "gameentity.hpp"

class Gun
{
public:
  bool isReloading = false;

  void Shot(GameEntity * shooter)
  {
    if (!isReloading)
    {
      m_bm.CreateBullet(shooter->GetBox(), m_bulletVelocity, m_bulletCaliber * m_bulletVelocity, Point2D(0.0f, 1.0f));
      if (--m_ammo == 0) Reload();
    }
  }

  void Reload()
  {
    isReloading = true;
    timer(3000); // 3 seconds
    isReloading = false;
//    m_ammo = CONSTANT_FROM_FILE;
  }

  void AccumulateScore(float damage) { m_score += damage; }

  void Launch(GameEntity * shooter)
  {
    if (m_score >= m_limit) m_bm.CreateMissile(shooter->GetBox(), m_missileVelocity, m_missileVelocity * m_missileCaliber, Point2D(0.0f, 1.0f));
    m_score -= m_limit;
  }

  float const GetLimit() const { return m_limit; }
  float const GetScore() const { return m_score; }

private:
  std::string m_name;
  int m_ammo = 10;
  float m_bulletCaliber = 1;
  float m_missileCaliber;
  int m_shootingpace = 50;
  float m_bulletVelocity;
  float m_missileVelocity;
  float m_limit;
  float m_score;
  BulletManager & m_bm;

  void timer(int ms)
  {
    int CLOCKS_PER_MSEC = CLOCKS_PER_SEC / 1000;
    clock_t end_time = clock() + ms * CLOCKS_PER_MSEC;
    while (clock() < end_time) {}
  }
};
