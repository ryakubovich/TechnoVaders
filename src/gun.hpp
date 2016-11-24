#pragma once

#include <string>

class Gun
{
public:
  void Reload();
  void Shot();
  float const GetLimit() const { return m_limit; }
  void Accumulate(float damage) { m_limit += damage; }

private:
  int m_ammo;
  float m_caliber;
  std::string m_name;
  int m_shootingpace;
  float m_limit;
};
