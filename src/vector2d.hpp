#pragma once

#include <cmath>
#include <initializer_list>

extern float const kEps;

class Vector2D
{
public:
  // Default constructor
  Vector2D() = default;

  // Constructor of two float coordinates
  Vector2D(float x, float y) : m_x(x), m_y(y) { std::cout << "2-float ctor" << std::endl; }

  // Constructor of many float coordinates
  Vector2D(std::initializer_list<float> const & lst)
  {
    std::cout << "Initializer list ctor" << std::endl;
    float * vals[] = { &m_x, &m_y };
    int count = sizeof(vals) / sizeof(vals[0]);
    auto it = lst.begin();
    for (int i = 0; i < count && it != lst.end(); i++, ++it)
    {
      *vals[i] = *it;
    }
  }

  // Copy semantics
  // Copy constructor
  Vector2D(Vector2D const & vec)
    : m_x(vec.m_x), m_y(vec.m_y) {}

  // Assignment operator
  Vector2D & operator = (Vector2D const & vec)
  {
    if (this == &vec) return *this;
    m_x = vec.m_x;
    m_y = vec.m_y;
    return *this;
  }

  // Move semantics
  Vector2D(Vector2D && vec)
    : m_x(vec.m_x), m_y(vec.m_y) { vec.m_x = 1.0f; vec.m_y = 0.0f; }

  Vector2D & operator = (Vector2D && vec)
  {
    std::swap(m_x, vec.m_x);
    std::swap(m_y, vec.m_y);
    return *this;
  }

  float & x() { return m_x; }
  float & y() { return m_y; }

  float const & x() const { return m_x; }
  float const & y() const { return m_y; }

  // Returns an angle in range [0; 2 * M_PI)
  static float CalculateAngle(Vector2D const & v1, Vector2D const & v2)
  {
    float scalarProduct = v1.Scalar(v2);
    float determinant = v1.Determinant(v2);
    float angle = atan2(determinant, scalarProduct);
    return angle < 0 ? 2 * M_PI + angle : angle;
  }

  // Logical equality operator
  bool operator == (Vector2D const & vec) const
  {
    if (m_x == vec.m_x && m_y == vec.m_y)
      return true;
    else
      return false;
  }

  // Logical inequality operator
  bool operator != (Vector2D const & vec) const
  {
    return !operator==(vec);
  }

  // Operator of adding a vector to *this
  Vector2D operator + (Vector2D const & vec)
  {
    return { m_x + vec.m_x, m_y + vec.m_y };
  }

  Vector2D & operator += (Vector2D const & vec)
  {
    m_x += vec.m_x;
    m_y += vec.m_y;
    return *this;
  }

  // Operator of subtracting a vector from *this
  Vector2D operator - (Vector2D const & vec) const
  {
    return { m_x - vec.m_x, m_y - vec.m_y };
  }

  Vector2D & operator -= (Vector2D const & vec)
  {
    m_x -= vec.m_x;
    m_y -= vec.m_y;
    return *this;
  }

  // Operator of multiplying vector by number
  Vector2D operator * (float scale)
  {
    return { m_x * scale, m_y * scale };
  }

  Vector2D & operator *= (float scale)
  {
    m_x *= scale;
    m_y *= scale;
    return *this;
  }

  // Operator of dividing vector by number
  Vector2D operator / (float scale)
  {
    if (EqualWithEps(0.0f, scale))
      return *this;
    else
      return { m_x / scale, m_y / scale };
  }

  Vector2D & operator /= (float scale)
  {
    if (EqualWithEps(0.0f, scale))
      return *this;
    else
    {
      m_x /= scale;
      m_y /= scale;
      return *this;
    }
  }

  // Nethod normalizes (length = 1) the vector by dividing its coordinates by its length
  void Normalize()
  {
    float length = Length();
    if (length > 0.0f)
    {
      m_x /= length;
      m_y /= length;
    }
  }

  // Method returns scalar product of two vectors
  float Scalar(Vector2D const & vec) const
  {
    return m_x * vec.m_x + m_y * vec.m_y;
  }

  // Method returns determinant of two vectors matrix
  float Determinant(Vector2D const & vec) const
  {
    return m_x * vec.m_y - m_y * vec.m_x;
  }

private:
  float m_x = 1.0f;
  float m_y = 0.0f;

  float Length() const
  {
    return sqrt(m_x * m_x + m_y * m_y);
  }

  bool EqualWithEps(float v1, float v2) const
  {
    return fabs(v1 - v2) < kEps;
  }
};

