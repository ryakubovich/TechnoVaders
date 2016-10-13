#pragma once

#include "vector2d.hpp"
#include "point2d.hpp"
#include "box2d.hpp"
#include <cmath>

class Ray2D
{
public:
  // Default constructor
  Ray2D() = default;

  // Constructor of Point2D and Vector2D variables
  Ray2D(Point2D origin, Vector2D direction) : m_origin(origin), m_direction(direction)
  {
    m_direction.Normalize();
  }

  // Copy semantics
  // Copy constructor
  Ray2D(Ray2D const & ray)
    : m_origin(ray.m_origin), m_direction(ray.m_direction) {}

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
    : m_origin(std::move(ray.m_origin)), m_direction(std::move(ray.m_direction)) {}

  Ray2D & operator = (Ray2D && ray)
  {
    m_origin = std::move(ray.m_origin);
    m_direction = std::move(ray.m_direction);
    return *this;
  }

  Point2D const & GetOrigin() const  { return m_origin; }
  Vector2D const & GetDirection() const  { return m_direction; }

  // Logical equality operator
  bool operator == (Ray2D const& ray) const
  {
    if (m_origin == ray.m_origin && m_direction == ray.m_direction)
      return true;
    else
      return false;
  }

  // Method checks whether a ray is intersecting a box and returns a bool result
  // Method uses technique of defining "vision scope" within which a ray intersects a box and checks if the ray direction is within this scope
  bool IsRayIntersectingBox(Box2D box)
  {
    Vector2D * xAxis = new Vector2D(1.0f, 0.0f);
    Vector2D * directionToOrigin = new Vector2D(m_origin.x(), m_origin.y());
    Point2D * allBoxPoints = box.GetAllPoints();
    float minAngle = 2.0f * M_PI + 1;
    float maxAngle = -1.0f;
    float angle;
    Vector2D * directionToPoint;
    Vector2D directionFromOriginToPoint;

    for (int i = 0; i < 4; i++)
    {
      directionToPoint = new Vector2D(allBoxPoints[i].x(), allBoxPoints[i].y());
      directionFromOriginToPoint = *directionToPoint - *directionToOrigin;
      directionFromOriginToPoint.Normalize();

      angle = Vector2D::CalculateAngle(*xAxis, directionFromOriginToPoint);

      if (angle < minAngle)
        minAngle = angle;
      if (angle > maxAngle)
        maxAngle = angle;
      delete directionToPoint;
    }

    angle = Vector2D::CalculateAngle(*xAxis, m_direction);
    delete directionToOrigin;
    delete xAxis;
    delete[] allBoxPoints;

    if (maxAngle - minAngle > M_PI)
    {
      if (angle >= maxAngle || angle <= minAngle)
        return true;
      else
        return false;
    }
    else
    {
      if (angle <= maxAngle && angle >= minAngle)
        return true;
      else
        return false;
    }
  }

private:
  Point2D m_origin = Point2D(0.0f, 0.0f);
  Vector2D m_direction = Vector2D(1.0f, 0.0f);
};
