#pragma once

#include "point2d.hpp"
#include "box2d.hpp"
#include <cmath>

class Ray2D
{
public:
  // Default constructor
  Ray2D() = default;

  // Constructor of Point2D and Point2D variables
  Ray2D(Point2D const & origin, Point2D const & direction)
    : m_origin(origin), m_direction(direction)
  {
    m_direction.Normalize();
  }

  // Copy semantics
  // Copy constructor
  Ray2D(Ray2D const & ray)
    : m_origin(ray.m_origin), m_direction(ray.m_direction)
  {}

  // Assignment operator
  Ray2D & operator = (Ray2D const & ray)
  {
    if (this == &ray) return *this;
    m_origin = ray.m_origin;
    m_direction = ray.m_direction;
    return *this;
  }

  // Move semantics
  Ray2D(Ray2D && ray)
    : m_origin(std::move(ray.m_origin)), m_direction(std::move(ray.m_direction))
  {}

  Ray2D & operator = (Ray2D && ray)
  {
    m_origin = std::move(ray.m_origin);
    m_direction = std::move(ray.m_direction);
    return *this;
  }

  Point2D const & GetOrigin() const { return m_origin; }
  Point2D const & GetDirection() const { return m_direction; }

  // Logical equality operator
  bool operator == (Ray2D const& ray) const
  {
    return m_origin == ray.m_origin && m_direction == ray.m_direction;
  }

  // Method checks whether a ray is intersecting a box and returns a bool result
  // Method uses technique of defining "vision scope" within which a ray intersects a box and checks if the ray direction is within this scope
  bool IsRayIntersectingBox(Box2D box)
  {
    Point2D xAxis(1.0f, 0.0f);
    Point2D directionToOrigin(m_origin.x(), m_origin.y());
    std::unique_ptr<Point2D[]> allBoxPoints = box.GetAllPoints();
    float minAngle = 2.0f * M_PI + 1;
    float maxAngle = -1.0f;
    float angle;
    Point2D directionToPoint;
    Point2D directionFromOriginToPoint;

    for (int i = 0; i < 4; i++)
    {
      directionToPoint = Point2D(allBoxPoints[i].x(), allBoxPoints[i].y());
      directionFromOriginToPoint = directionToPoint - directionToOrigin;
      directionFromOriginToPoint.Normalize();

      angle = Point2D::CalculateAngle(xAxis, directionFromOriginToPoint);

      if (angle < minAngle) minAngle = angle;
      if (angle > maxAngle) maxAngle = angle;
    }

    angle = Point2D::CalculateAngle(xAxis, m_direction);

    if (maxAngle - minAngle > M_PI) return angle >= maxAngle || angle <= minAngle;
    else return angle <= maxAngle && angle >= minAngle;
  }

private:
  Point2D m_origin = Point2D(0.0f, 0.0f);
  Point2D m_direction = Point2D(0.0f, 0.0f);
};
