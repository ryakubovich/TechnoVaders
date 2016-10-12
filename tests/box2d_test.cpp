#include "gtest/gtest.h"
#include "box2d.hpp"
#include "point2d.hpp"
#include <iostream>

using namespace std;

TEST(box2d_test, test_construction)
{
  //Задание по умолчанию
  Box2D box1;
  EXPECT_EQ(box1, Box2D(Point2D(0.0f, 0.0f), Point2D(0.0f, 0.0f)));

  Box2D box2;
  EXPECT_EQ(box2, Box2D(0.0f, 0.0f, 0.0f, 0.0f));

  //Проверка совпадения задания через две точки и через четыре числа
  Box2D box3 = Box2D(1.0f, 3.0f, 5.0f, 6.0f);
  EXPECT_EQ(box3, Box2D(Point2D(1.0f, 3.0f), Point2D(5.0f, 6.0f)));

  //To Do: Проверка создания прямоугольника с обратным порядком точек
  Point2D p1 = { 4.0f, 5.0f };
  Point2D p2 = { 3.0f, 1.0f } ;
  box2 = Box2D(p1, p2);
//  box3 = Box2D(p1, p2);
//  std::cout << "Point 1 " << p1 << " Point " << p2 << std::endl;
//  std::cout << "In test " << box2.p1() << " " << box2.p2() << std::endl;
//  std::cout << "In test " << box3.p1() << " " << box3.p2() << std::endl;
  EXPECT_EQ(box2, Box2D(Point2D(3.0f, 1.0f), Point2D(4.0f, 5.0f)));
}

TEST(box2d_test, test_scale)
{
  //Проверка неподвижности центра при масштабировании
  Box2D box1 = {Point2D(2.0f, 1.0f), Point2D(8.0f, 4.0f)};
  Point2D center1 = box1.GetCenter();
  box1 /= 2.0f;
  Point2D center2 = box1.GetCenter();
  box1 *= 2.0f;
  Point2D center3 = box1.GetCenter();
  EXPECT_EQ(center1, center2);
  EXPECT_EQ(center2, center3);
}

TEST(box2d_test, test_intersection)
{
  //Проверка пересечения двух дефолтных (нулевых) прямоугольников
  Box2D box1 = Box2D();
  EXPECT_EQ(box1.IsBoxIntersectingBox(Box2D()), true);

  //Проверка пересечения двух длинных прямоугольников крест накрест
  Box2D box2 = Box2D(4.0f, 1.0f, 5.0f, 5.0f);
  EXPECT_EQ(box2.IsBoxIntersectingBox(Box2D(1.0f, 2.0f, 7.0f, 3.0f)), true);

  //Проверка пересечения вложенных прямоугольников
  EXPECT_EQ(box2.IsBoxIntersectingBox(Box2D(4.3f, 2.0f, 4.7f, 4.0f)), true);

  //Проверка пересечения прямоугольников после масштабирования (уменьшение)
  Box2D box3 = {Point2D(6.0f, 2.0f), Point2D(8.0f, 10.0f)};
  Box2D box4 = Box2D(0.0f, 0.0f, 8.0f, 3.0f);
  std::cout << "Initial coords: " << box3.p1() << " " << box3.p2() << std::endl;
  EXPECT_EQ(box3.IsBoxIntersectingBox(box4), true);
  box3 /= 3.0f;
  std::cout << "Coords after zooming out: " << box3.p1() << " " << box3.p2() << std::endl;
  EXPECT_EQ(box3.IsBoxIntersectingBox(box4), false);

  //Проверка пересечения прямоугольников после масштабирования (увеличение)
  box3 *= 3.0f;
  std::cout << "Coords after zooming in: " << box3.p1() << " " << box3.p2() << std::endl;
  EXPECT_EQ(box3.IsBoxIntersectingBox(box4), true);

}
