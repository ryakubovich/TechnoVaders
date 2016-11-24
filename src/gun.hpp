#pragma once

class Gun
{
public:
  void Reaload();
  void Shot();

  int m_ammo;
  float m_caliber;
  string m_name;
  int m_shootingpace;
};



