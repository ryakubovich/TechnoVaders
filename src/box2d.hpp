#pragma once

#include <initializer_list>
#include "point2d.hpp"
#include <memory>
#include <iostream>

class Box2D
{
public:
  // Default constructor
  Box2D() = default;

  // Constructor of two Point2D objects
  Box2D(Point2D const & obj1, Point2D const & obj2)
    : m_min(obj1), m_max(obj2)
  {
    WrongOrderFix();
  }

  // Constructor of four float coordinates
  Box2D(float x1, float y1, float x2, float y2)
    : m_min(Point2D(x1, y1)), m_max(Point2D(x2, y2))
  {
    WrongOrderFix();
  }

  // Constructor of many Point2D objects
  Box2D (std::initializer_list<Point2D> const & lst)
  {
    Point2D * vals[] = { &m_min, &m_max };
    int const count = sizeof(vals) / sizeof(vals[0]);
    auto it = lst.begin();
    for (int i=0; i < count && it != lst.end(); i++, ++it)
      *vals[i] = *it;
    WrongOrderFix();
  }

  // Copy semantics
  // Copy constructor
  Box2D(Box2D const & obj)
    : m_min(obj.m_min), m_max(obj.m_max)
  {}

  // Assignment operator
  Box2D & operator = (Box2D const & obj)
  {
    if (this == &obj) return *this;
    m_min = obj.GetMin();
    m_max = obj.GetMax();
    return *this;
  }

  // Move semantics
  Box2D(Box2D && obj)
    : m_min(std::move(obj.m_min)), m_max(std::move(obj.m_max)) {}

  Box2D & operator = (Box2D && obj)
  {
    m_min = std::move(obj.m_min);
    m_max = std::move(obj.m_max);
    return *this;
  }

  // Logical equality operator
  bool operator == (Box2D const & obj) const
  {
    return EqualWithEps(GetMin().x(), obj.GetMin().x()) &&
        EqualWithEps(GetMin().y(), obj.GetMin().y()) &&
        EqualWithEps(GetMax().x(), obj.GetMax().x()) &&
        EqualWithEps(GetMax().y(), obj.GetMax().y());
  }

  // Logical inequality operator
  bool operator != (Box2D const & obj) const
  {
    return !operator==(obj);
  }

  // Scaling up operator
  Box2D & operator * (float scale)
  {
    float length_x = m_max.x() - m_min.x();
    float length_y = m_max.y() - m_min.y();
    m_min = Point2D(m_min.x() - length_x * (scale - 1) / 2.0f, m_min.y() - length_y * (scale - 1) / 2.0f);
    m_max = Point2D(m_max.x() + length_x * (scale - 1) / 2.0f, m_max.y() + length_y * (scale - 1) / 2.0f);
    return *this;
  }

  Box2D & operator *= (float scale)
  {
    float length_x = m_max.x() - m_min.x();
    float length_y = m_max.y() - m_min.y();
    m_min = Point2D(m_min.x() - length_x * (scale - 1) / 2.0f, m_min.y() - length_y * (scale - 1) / 2.0f);
    m_max = Point2D(m_max.x() + length_x * (scale - 1) / 2.0f, m_max.y() + length_y * (scale - 1) / 2.0f);
    return *this;
  }

  // Scaling down operator
  Box2D & operator / (float scale)
  {
    // Checks for division by zero or negative number
    if (scale < kEps)
    {
      std::cout << "Box scale err: div by zero or by negative float" << std::endl;
      return *this;
    }
    float length_x = m_max.x() - m_min.x();
    float length_y = m_max.y() - m_min.y();
    m_min = Point2D(m_min.x() + length_x * (1.0f - 1.0f / scale) / 2.0f, m_min.y() + length_y * (1.0f - 1.0f / scale) / 2.0f);
    m_max = Point2D(m_max.x() - length_x * (1.0f - 1.0f / scale) / 2.0f, m_max.y() - length_y * (1.0f - 1.0f / scale) / 2.0f);
    return *this;
  }

  Box2D & operator /= (float scale)
  {
    // Checks of division by zero or negative number
    if (scale < kEps)
    {
      std::cout << "Box scale err: div by zero or by negative float" << std::endl;
      return *this;
    }
    float length_x = m_max.x() - m_min.x();
    float length_y = m_max.y() - m_min.y();
    m_min = Point2D(m_min.x() + length_x * (1.0f - 1.0f / scale) / 2.0f, m_min.y() + length_y * (1.0f - 1.0f / scale) / 2.0f);
    m_max = Point2D(m_max.x() - length_x * (1.0f - 1.0f / scale) / 2.0f, m_max.y() - length_y * (1.0f - 1.0f / scale) / 2.0f);
    return *this;
  }

  // Return all box points in std::unique_ptr<Point2D[]> type
  std::unique_ptr<Point2D[]> GetAllPoints() const
  {
    return std::unique_ptr<Point2D[]>(new Point2D[4]{m_min, Point2D(m_min.x(), m_max.y()), Point2D(m_max.x(), m_min.y()), m_max});
  }

  // Method checks whether boxes intersect each other or not
  bool IsBoxIntersectingBox(Box2D const & b) const
  {
    if (GetMax().x() < b.GetMin().x()) return false;
    if (GetMin().x() > b.GetMax().x()) return false;
    if (GetMin().y() > b.GetMax().y()) return false;
    if (GetMax().y() < b.GetMin().y()) return false;
    return true;
  }

  // Return box center
  Point2D GetCenter() const
  {
    return Point2D((m_min.x() + m_max.x()) / 2.0f, (m_min.y() + m_max.y()) / 2.0f);
  }

  Point2D const & GetMin() const { return m_min; }
  Point2D const & GetMax() const { return m_max; }

  void SetMin(Point2D const & point) { m_min = point; }
  void SetMax(Point2D const & point) { m_max = point; }

  friend std::ostream & operator << (std::ostream & os, Box2D box)
  {
    os << "Box: { Min = " << box.m_min << " ; Max = " << box.m_max << " }";
    return os;
  }

private:
  Point2D m_min = Point2D(0.0f, 0.0f);
  Point2D m_max = Point2D(0.0f, 0.0f);

  // Method checks whether min point has a smaller x coordinate and a smaller y coordinate than max and fixes it if possible
  void WrongOrderFix()
  {
    if (m_min.x() == m_max.x() || m_min.y() == m_max.y())
    {
      std::cout << "Some coordinates are the same for both points, points are being switched to default values"
                << std::endl;
      m_min = m_max = Point2D();
    }
    else
    {
      Point2D p1 = m_min;
      Point2D p2 = m_max;
      m_min.x() = std::min(p1.x(), p2.x());
      m_min.y() = std::min(p1.y(), p2.y());
      m_max.x() = std::max(p1.x(), p2.x());
      m_max.y() = std::max(p1.y(), p2.y());
    }
  }

  bool EqualWithEps(float v1, float v2) const
  {
    return fabs(v1 - v2) < kEps;
  }

  float m_x = 0.0f, m_y = 0.0f;
};
