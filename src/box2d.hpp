#pragma once

#include <initializer_list>
#include "point2d.hpp"

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
  Box2D(float x_1, float y_1, float x_2, float y_2)
    : m_min(Point2D(x_1, y_1)), m_max(Point2D(x_2, y_2))
  {
    WrongOrderFix();
  }

  // Constructor of many Point2D objects
  Box2D (std::initializer_list<Point2D> const & lst)
  {
    std::cout << "Initializer list ctor" << std::endl;
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
    :m_min(obj.m_min), m_max(obj.m_max)
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
    bool p1x = EqualWithEps(GetMin().x(), obj.GetMin().x());
    bool p1y = EqualWithEps(GetMin().y(), obj.GetMin().y());
    bool p2x = EqualWithEps(GetMax().x(), obj.GetMax().x());
    bool p2y = EqualWithEps(GetMax().y(), obj.GetMax().y());
    return p1x && p1y && p2x && p2y;
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
    if (scale < kEps) { std::cout << "Box scale err: div by zero or by negative float" << std::endl; return *this; }
    float length_x = m_max.x() - m_min.x();
    float length_y = m_max.y() - m_min.y();
    m_min = Point2D(m_min.x() + length_x * (1.0f - 1.0f / scale) / 2.0f, m_min.y() + length_y * (1.0f - 1.0f / scale) / 2.0f);
    m_max = Point2D(m_max.x() - length_x * (1.0f - 1.0f / scale) / 2.0f, m_max.y() - length_y * (1.0f - 1.0f / scale) / 2.0f);
    return *this;
  }

  Box2D & operator /= (float scale)
  {
    // Checks of division by zero or negative number
    if (scale < kEps) { std::cout << "Box scale err: div by zero or by negative float" << std::endl; return *this; }
    float length_x = m_max.x() - m_min.x();
    float length_y = m_max.y() - m_min.y();
    m_min = Point2D(m_min.x() + length_x * (1.0f - 1.0f / scale) / 2.0f, m_min.y() + length_y * (1.0f - 1.0f / scale) / 2.0f);
    m_max = Point2D(m_max.x() - length_x * (1.0f - 1.0f / scale) / 2.0f, m_max.y() - length_y * (1.0f - 1.0f / scale) / 2.0f);
    return *this;
  }

  // Return all box points in Point2D* type
  Point2D * GetAllPoints() const
  {
    Point2D * pointsArray = new Point2D[4]{m_min, Point2D(m_min.x(), m_max.y()), Point2D(m_max.x(), m_min.y()), m_max};
    return pointsArray;
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
    return Point2D((m_min.x() + m_max.x())/ 2.0f, (m_min.y() + m_max.y())/ 2.0f);
  }

  Point2D const & GetMin() const { return m_min; }
  Point2D const & GetMax() const { return m_max; }

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
      bool x = m_min.x() > m_max.x();
      bool y = m_min.y() > m_max.y();
      if (x && y)
      {
        Point2D tmp = m_min;
        m_min = Point2D(m_max.x(), m_max.y());
        m_max = tmp;
      }
      else if (x)
      {
        float tmp = m_min.x();
        m_min = Point2D(m_max.x(), m_min.y());
        m_max = Point2D(tmp, m_max.y());
      }
      else if (y)
      {
        float tmp = m_min.y();
        m_min = Point2D(m_min.x(), m_max.y());
        m_max = Point2D(m_max.x(), tmp);
      }
    }
  }

  bool EqualWithEps(float v1, float v2) const
  {
    return fabs(v1 - v2) < kEps;
  }

  float m_x = 0.0f, m_y = 0.0f;
};
