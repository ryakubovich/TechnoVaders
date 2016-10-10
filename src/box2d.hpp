#pragma once

#include <initializer_list>
#include "point2d.hpp"

//float const kEps=1e-5;

class Box2D
{
public:
    //Конструктор по умолчанию
    Box2D() = default;

    //Конструктор копирования
    Box2D(Box2D const & obj)
        :pnt1(obj.pnt1), pnt2(obj.pnt2)
    {}

    //Конструктор с параметрами Points
    Box2D (Point2D const& obj1, Point2D const& obj2)
        : pnt1(obj1), pnt2(obj2)
    {}

    //Конструктор с параметрами - координаты точек
    Box2D(float x_1, float y_1, float x_2, float y_2)
        : pnt1(Point2D(x_1, y_1)), pnt2(Point2D(x_2, y_2))
    {
        //Обработка обратных точек
        bool x = x_1 > x_2;
        bool y = y_1 > y_2;
        if (x&&y) { pnt1(Point2D(x_2 ,y_2)); pnt2(Point2D(x_1, y_1)); }
        if (x) { pnt1(Point2D(x_2 ,y_1)); pnt2(Point2D(x_1, y_2)); }
        if (y) { pnt1(Point2D(x_1 ,y_2)); pnt2(Point2D(x_2, y_1)); }
    }

    //Конструирование через initializer_list
    Box2D (std::initializer_list<Point2D> const & lst)
    {
        Point2D * vals[] = { &pnt1, &pnt2 };
        int const count = sizeof(vals) / sizeof(vals[0]);

        auto it = lst.begin();
        for (int i=0; i < count && it != lst.end(); i++, ++it)
            *vals[i] = *it;
        //Обработка обратных точек
        if (pnt1 > pnt2) { vals[0] = &pnt2; vals[1] = &pnt1; }
        //Конец
    }

    // Оператор логического равенства.
    bool operator == (Box2D const & obj) const
    {
      bool p1x = EqualWithEps(p1().x(), obj.p1().x());
      bool p1y = EqualWithEps(p1().y(), obj.p1().y());
      bool p2x = EqualWithEps(p2().x(), obj.p2().x());
      bool p2y = EqualWithEps(p2().y(), obj.p2().y());
      return p1x && p1y && p2x && p2y;
    }

    // Оператор присваивания.
    Box2D & operator = (Box2D const & obj)
    {
      if (this == &obj) return *this;
      pnt1 = obj.p1();
      pnt2 = obj.p2();
      return *this;
    }

    // Оператор логического неравенства.
    bool operator != (Box2D const & obj) const
    {
      return !operator==(obj);
    }

    //Оператор увеличения
    Box2D & operator * (float scale)
    {
      float length_x = pnt2.m_x - pnt1.m_x;
      float length_y = pnt2.m_y - pnt1.m_y;
      pnt1.m_x = pnt1.m_x - length_x * (scale - 1) / 2.0f;
      pnt1.m_y = pnt1.m_y - length_y * (scale - 1) / 2.0f;
      pnt2.m_x = pnt2.m_x + length_x * (scale - 1) / 2.0f;
      pnt2.m_y = pnt2.m_y + length_y * (scale - 1) / 2.0f;
      return *this;
    }

    //Оператор уменьшения
    Box2D & operator / (float scale)
    {
      //Деление на ноль
        if (scale < kEps) { std::cout << "Box scale err = div by zero or by negative float" << std::endl; return *this; }
      float length_x = pnt2.m_x - pnt1.m_x;
      float length_y = pnt2.m_y - pnt1.m_y;
      pnt1.m_x = pnt1.m_x + length_x * (scale - 1) / 2.0f;
      pnt1.m_y = pnt1.m_y + length_y * (scale - 1) / 2.0f;
      pnt2.m_x = pnt2.m_x - length_x * (scale - 1) / 2.0f;
      pnt2.m_y = pnt2.m_y - length_y * (scale - 1) / 2.0f;
      return *this;
    }

    Point2D * GetAllPoints() const
    {
      Point2D * pointsArray = new Point2D[4]{pnt1, Point2D(pnt1.x(), pnt2.y()), Point2D(pnt2.x(), pnt1.y()), pnt2};
      return pointsArray;
    }

    //Метод проверки пересечения
    bool IsBoxIntersectingBox(const Box2D &b)
    {
        if (p2().x() < b.p1().x()) return false; // to the left
        if (p1().x() > b.p2().x()) return false; // to the right
        if (p1().y() > b.p2().y()) return false; // above
        if (p2().y() < b.p1().y()) return false; // below
        return true; // boxes overlap
    }

    //Нахождение центра
    Point2D const GetCenter(Box2D const & obj) {
        return Point2D( ( obj.p1().x() + obj.p2().x() )/2 , ( obj.p1().y() + obj.p2().y() )/2 );
    }

    Point2D &p1() { return pnt1; }
    Point2D &p2() { return pnt2; }

    Point2D const& p1() const { return pnt1; }
    Point2D const& p2() const { return pnt2; }


private:
    //Задание пары точек
    Point2D pnt1 = Point2D(0.0f, 0.0f);
    Point2D pnt2 = Point2D(0.0f, 0.0f);

    bool EqualWithEps(float v1, float v2) const
    {
      return fabs(v1 - v2) < kEps;
    }

    float m_x = 0.0f, m_y = 0.0f;
};
