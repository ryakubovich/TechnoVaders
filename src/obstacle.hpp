#pragma once

#include "box2d.hpp"
#include <vector>

class Obstacle
{
public:
  void Hit();

  Box2D const & GetOverallBox() const
  {
    auto it = m_subObstacles.begin();
    min = it->GetMin();
    max = it->GetMax();
    ++it;
    for (; it != m_subObstacles.end(); ++it)
    {
      min = std::min(min, it->GetMin());
      max = std::max(max, it->GetMax());
    }
    return { min, max };
  }

private:
  std::vector<Box2D> m_subObstacles;

  void Deformation();
};
