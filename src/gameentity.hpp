#pragma once

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

  void ResizeBox(float kWidth, float kHeight)
  {
    m_box.SetMin(Point2D(m_box.GetMin().x() * kWidth, m_box.GetMin().y() * kHeight));
    m_box.SetMax(Point2D(m_box.GetMax().x() * kWidth, m_box.GetMax().y() * kHeight));
  }

  Box2D const & GetBox() const { return m_box; }

protected:
  Box2D m_box;
};
