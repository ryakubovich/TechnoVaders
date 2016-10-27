#pragma once

#include "box2d.hpp"

class GameEntity
{
public:

  void Move(Point2D offset)
  {
    m_box.SetMin(m_box.GetMin() + offset);
    m_box.SetMax(m_box.GetMax() + offset);
  }

private:
  Box2D m_box;
};
