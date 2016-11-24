#pragma once

#include "alien.hpp"
#include <list>

class AI
{
public:
  void Update();
  void Shot();
  std::list<Alien> const & GetAliens() const { return m_aliens; }

private:
  std::list<Alien> m_aliens;
};

