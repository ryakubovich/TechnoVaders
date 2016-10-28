#pragma once

#include "box2d.hpp"

class GameEntity
{
public:
  GameEntity(box)
    : m_box(box)
  {}

  GameEntity(Point2D min, Point2D max)
    : m_box(min, max)
  {}

  void Move(Point2D offset)
  {
    m_box.SetMin(m_box.GetMin() + offset);
    m_box.SetMax(m_box.GetMax() + offset);
  }

  Box2D const & GetBox() const { return m_box; }

private:
  Box2D m_box;
};
