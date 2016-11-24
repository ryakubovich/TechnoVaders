#pragma once

#include "box2d.hpp"
#include <vector>

class Obstacle
{
public:
  Obstacle(Point2D min, Point2D max, float heightSub, float widthSub)
  {
    for (int i = min.x(); i <= max.x() - widthSub; i += widthSub)
      for (int j = min.y(); j <= max.y() - heightSub; j += heightSub)
        m_subObstacles.emplace_back(Box2D(Point2D(i, j), Point2D(i + widthSub, j + heightSub)));
  }

  void Hit(Box2D const & hitBox)
  {
    for (auto it = m_subObstacles.begin(); it != m_subObstacles.end(); ++it)
    {
      if (it->IsBoxIntersectingBox(hitBox))
        m_subObstacles.erase(it);
    }
  }

  Box2D const GetOverallBox() const
  {
    auto it = m_subObstacles.begin();
    Point2D min = it->GetMin();
    Point2D max = it->GetMax();
    for (; ++it != m_subObstacles.end();)
    {
      min = std::min(min, it->GetMin());
      max = std::max(max, it->GetMax());
    }
    return { min, max };
  }

private:
  std::vector<Box2D> m_subObstacles;
};
