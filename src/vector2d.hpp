#pragma once

#include <cmath>
#include <initializer_list>

class Vector2D
{
public:
  Vector2D() = default;

  Vector2D(float x, float y) : m_x(x), m_y(y) {}

//  Vector2D(std::initializer_list<float> const& lst)
//  {
//    float * vals[] = { &m_x, &m_y };
//    it = lst.begin();
//    for (int i=0; i < count && it != lst.end(); i++, ++it)
//    {
//      *vals[i] = *it;
//    }
//  }

  float & x() { return m_x; }
  float & y() { return m_y; }

  float const & x() const { return m_x; }
  float const & y() const { return m_y; }

  Vector2D operator + (Vector2D const & vec)
  {
    return { m_x + vec.m_x, m_y + vec.m_y };
  }

  Vector2D operator += (Vector2D const & vec)
  {
    return *this + vec;
  }

  Vector2D operator - (Vector2D const & vec)
  {
    return { m_x - vec.m_x, m_y - vec.m_y };
  }

  Vector2D operator -= (Vector2D const & vec)
  {
    return *this - vec;
  }

  Vector2D operator * (float scale)
  {
    return { m_x * scale, m_y * scale };
  }

  Vector2D operator *= (float scale)
  {
    return *this * scale;
  }

  void Normalize()
  {
    float length = Length();
    m_x /= length;
    m_y /= length;
  }

  float Scalar(Vector2D const & vec)
  {
    return m_x * vec.m_x + m_y * vec.m_y;
  }

private:
  float m_x = 0.0f;
  float m_y = 0.0f;

  float Length()
  {
    return sqrt(m_x * m_x + m_y * m_y);
  }
};
