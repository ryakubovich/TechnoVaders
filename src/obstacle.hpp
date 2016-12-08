#pragma once

#include "box2d.hpp"
#include <list>

using TSubObstacles = std::list<Box2D>;

class Obstacle
{
public:
  Obstacle(Point2D const & min, Point2D const & max, float widthSub, float heightSub)
  {
    for (int i = min.x(); i <= max.x() - widthSub; i += widthSub)
      for (int j = min.y(); j <= max.y() - heightSub; j += heightSub)
        m_subObstacles.emplace_back(Box2D(Point2D(i, j), Point2D(i + widthSub, j + heightSub)));
  }

  bool Damage(Box2D const & hitBox)
  {
    for (auto it = m_subObstacles.begin(); it != m_subObstacles.end(); ++it)
      if (it->IsBoxIntersectingBox(hitBox))
      {
        m_subObstacles.erase(it);
        break;
      }
    return m_subObstacles.size() == 0;
  }

  // TO DO: process pits
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

  TSubObstacles const & GetSubs() const { return m_subObstacles; }

  friend std::ostream & operator << (std::ostream & os, Obstacle const & obstacle)
  {
    os << "Obstacle: { Overall Box = " << obstacle.GetOverallBox() << " ; Small obstacles = [";
    for (auto const & subObstacle: obstacle.m_subObstacles)
      os << subObstacle << "," << std::endl;
    os << "]}";
    return os;
  }

private:
  TSubObstacles m_subObstacles;
};
