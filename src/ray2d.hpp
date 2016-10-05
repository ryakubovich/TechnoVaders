#pragma once

#include <vector2d.hpp>
#include <point2d.hpp>

class Ray2D
{
public:
  Ray2D() = default;

  Ray2D(Point2D origin, Vector2D direction) : m_origin(origin), m_direction(direction) {}

private:
  Point2D m_origin = Point2D(0.0f, 0.0f);
  Vector2D m_direction = Vector2D(0.0f, 0.0f);
};
