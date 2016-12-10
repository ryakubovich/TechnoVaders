#pragma once

#include "box2d.hpp"
#include <list>

using TSubObstacles = std::list<Box2D>;

enum DamageType
{
  NoDamage,
  Damaged,
  Destroyed
};

class Obstacle
{
public:
  Obstacle(Point2D const & min, Point2D const & max, float widthSub, float heightSub/*, int screenWidth, int screenHeight*/)
    : m_overallBox(min, max)/*, m_kWidth(screenWidth / 1366.0f), m_kHeight(screenHeight / 768.0f)*/
  {
    for (int i = min.x(); i <= max.x() - widthSub; i += widthSub)
      for (int j = min.y(); j <= max.y() - heightSub; j += heightSub)
        m_subObstacles.emplace_back(Box2D(Point2D(i, j), Point2D(i + widthSub, j + heightSub)));
  }

  int Damage(Box2D const & hitBox)
  {
    for (auto it = m_subObstacles.begin(); it != m_subObstacles.end(); ++it)
      if (it->IsBoxIntersectingBox(hitBox))
      {
        m_subObstacles.erase(it);
        return DamageType::Damaged;
      }
    if (m_subObstacles.size() == 0) return DamageType::Destroyed;
    else return DamageType::NoDamage;
  }

  Box2D const & GetOverallBox() const
  {
    return m_overallBox;
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

//  void Resized(int width, int height)
//  {
//    m_kWidth = (width / 1366.0f) / m_kWidth;
//    m_kHeight = (height / 768.0f) / m_kHeight;
//    for (auto & sub : m_subObstacles)
//    {
//      sub.SetMin(Point2D(sub.GetMin().x() * m_kWidth, sub.GetMin().y() * m_kHeight));
//      sub.SetMax(Point2D(sub.GetMax().x() * m_kWidth, sub.GetMax().y() * m_kHeight));
//    }
//    m_overallBox.SetMin(Point2D(m_overallBox.GetMin().x() * m_kWidth, m_overallBox.GetMin().y() * m_kHeight));
//    m_overallBox.SetMax(Point2D(m_overallBox.GetMax().x() * m_kWidth, m_overallBox.GetMax().y() * m_kHeight));
//  }

private:
  Box2D m_overallBox;
  TSubObstacles m_subObstacles;
//  float m_kWidth;
//  float m_kHeight;
};
