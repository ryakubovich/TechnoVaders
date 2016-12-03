#pragma once

#include "logger.hpp"
#include "box2d.hpp"

class GameEntity
{
public:
  GameEntity() = default;

  GameEntity(Box2D const & box)
    : m_box(box)
  {}

  GameEntity(Point2D const & min, Point2D const & max)
    : m_box(min, max)
  {}

  void Move(Point2D const & offset)
  {
    m_box.SetMin(m_box.GetMin() + offset);
    m_box.SetMax(m_box.GetMax() + offset);
  }

  Box2D const & GetBox() const { return m_box; }

protected:
  Box2D m_box;
};
