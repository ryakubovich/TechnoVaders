#pragma once

#include <string>
#include <ctime>

class Gun
{
public:
  bool reload = false;

  void Shot()
  {
    CreateBullet();
    m_ammo --;
  }

  void Reload()
  {
    reload = true;
    timer(3000); // 3 seconds
    reload = false;
  }

  float const GetLimit() const { return m_limit; }

  void AccumulateScore(float damage)
  {
    damage = m_caliber * m_shotingpace;
    m_limit += damage;
  }

  float const GetScore() const { return m_score; }

  void Launch()
  {
    if (m_score == m_limit)
    {
      LaunchMissile();
    }
  }

  void timer(int ms)
  {
    int CLOCKS_PER_MSEC = CLOCKS_PER_SEC / 1000;
    clock_t end_time = clock() + ms * CLOCKS_PER_MSEC;
    while (clock() < end_time) {}
  }

private:
  int m_ammo = 10;
  float m_caliber = 1;
  std::string m_name;
  int m_shootingpace = 50;
  float m_limit;
  float m_score;
};
