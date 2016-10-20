#pragma once

#include <cmath>
#include <initializer_list>
#include <functional>
#include <ostream>

float const kEps = 1e-5;

class Point2D
{
public:
  // Разрешаем конструирование по умолчанию.
  Point2D() = default;

  // Конструктор с параметрами.
  Point2D(float x, float y)
    : m_x(x), m_y(y)
  {}

  // Copy semantics
  Point2D(Point2D const & obj)
    : m_x(obj.m_x), m_y(obj.m_y)
  {}

  Point2D & operator = (Point2D const & obj)
  {
    if (this == &obj) return *this;
    m_x = obj.m_x;
    m_y = obj.m_y;
    return *this;
  }

  // Move semantics
  Point2D(Point2D && obj)
    : m_x(obj.m_x), m_y(obj.m_y) { obj.m_x = obj.m_y = 0; }

  Point2D & operator = (Point2D && obj)
  {
    std::swap(m_x, obj.m_x);
    std::swap(m_y, obj.m_y);
    return *this;
  }

  // Оператор логического равенства.
  bool operator == (Point2D const & obj) const
  {
    return EqualWithEps(m_x, obj.m_x) && EqualWithEps(m_y, obj.m_y);
  }

  float & x() { return m_x; }
  float & y() { return m_y; }

  float const & x() const { return m_x; }
  float const & y() const { return m_y; }

  // Конструктор со списком инициализации.
  Point2D(std::initializer_list<float> const & lst)
  {
    float * vals[] = { &m_x, &m_y };
    int const count = sizeof(vals) / sizeof(vals[0]);
    auto it = lst.begin();
    for (int i = 0; i < count && it != lst.end(); i++, ++it)
      *vals[i] = *it;
  }

  // Оператор логического неравенства.
  bool operator != (Point2D const & obj) const
  {
    return !operator==(obj);
  }

  // Оператор меньше.
  bool operator < (Point2D const & obj) const
  {
    if (m_x != obj.m_x) return m_x < obj.m_x;
    return m_y < obj.m_y;
  }

  // Сложение.
  Point2D operator + (Point2D const & obj) const
  {
    return { m_x + obj.m_x, m_y + obj.m_y };
  }

  // Вычитание.
  Point2D operator - (Point2D const & obj) const
  {
    return { m_x - obj.m_x, m_y - obj.m_y };
  }

  // Математическое отрицание.
  Point2D operator - () const
  {
    return { -m_x, -m_y };
  }

  // Умножение на число.
  Point2D operator * (float scale) const
  {
    return { m_x * scale, m_y * scale };
  }

  // Деление на число.
  Point2D operator / (float scale) const
  {
    if (EqualWithEps(scale, 0.0f))
      return *this;
    return { m_x / scale, m_y / scale };
  }

  Point2D & operator += (Point2D const & obj)
  {
    m_x += obj.m_x;
    m_y += obj.m_y;
    return *this;
  }

  Point2D & operator -= (Point2D const & obj)
  {
    m_x -= obj.m_x;
    m_y -= obj.m_y;
    return *this;
  }

  Point2D & operator *= (float scale)
  {
    m_x *= scale;
    m_y *= scale;
    return *this;
  }

  Point2D & operator /= (float scale)
  {
    if (EqualWithEps(scale, 0.0f))
      return *this;
    m_x /= scale;
    m_y /= scale;
    return *this;
  }

  // Переопределение оператора [].
  float operator [] (unsigned int index) const
  {
    if (index >= 2) return 0.0f;
    return index == 0 ? m_x : m_y;
  }

  // Добавим внутреннюю сущность для вычисления хэша.
  struct Hash
  {
    size_t operator()(Point2D const & p) const
    {
      auto hasher = std::hash<float>();
      return (hasher(p.x()) ^ (hasher(p.y()) << 1));
    }
  };

  static float CalculateAngle(Point2D const & vector1, Point2D const & vector2)
  {
    float scalarProduct = vector1.Scalar(vector2);
    float determinant = vector1.Determinant(vector2);
    float angle = atan2(determinant, scalarProduct);
    return angle < 0 ? 2 * M_PI + angle : angle;
  }

  // Nethod normalizes (length = 1) the vector (point) by dividing its coordinates by its length
  void Normalize()
  {
    float length = Length();
    if (!EqualWithEps(0.0f, length))
    {
      m_x /= length;
      m_y /= length;
    }
  }

  // Method returns scalar product of two vectors
  float Scalar(Point2D const & vec) const
  {
    return m_x * vec.m_x + m_y * vec.m_y;
  }

  // Method returns determinant of two vectors matrix
  float Determinant(Point2D const & vec) const
  {
    return m_x * vec.m_y - m_y * vec.m_x;
  }

private:

  bool EqualWithEps(float v1, float v2) const
  {
    return fabs(v1 - v2) < kEps;
  }

  float Length() const
  {
    return sqrt(m_x * m_x + m_y * m_y);
  }

  float m_x = 0.0f, m_y = 0.0f;
};

inline std::ostream & operator << (std::ostream & os, Point2D const & obj)
{
  os << "Point2D {" << obj.x() << ", " << obj.y() << "}";
  return os;
}
