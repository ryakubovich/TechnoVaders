#include "gtest/gtest.h"
#include "ray2d.hpp"
#include <iostream>

using namespace std;

TEST(ray2d_test, test_contruction)
{
  Ray2D r1;
  EXPECT_EQ(r1, Ray2D(Point2D(0.0f, 0.0f),
                      Vector2D(1.0f, 0.0f)));

  Ray2D r2 = { Point2D(5.0f, 5.0f),
               Vector2D(2.0f, 0.0f) };
  EXPECT_EQ(r2, Ray2D(Point2D(5.0f, 5.0f),
                      Vector2D(1.0f, 0.0f)));

  Ray2D r3 = { Point2D(1.0f, 1.0f),
               Vector2D(1.0f, 1.0f) };
  r3 = std::move(r1);
  Vector2D v1 = { 1.0f, 1.0f };
  v1.Normalize();
  EXPECT_EQ(r3.origin(), Point2D(0.0f, 0.0f));
  EXPECT_EQ(r3.direction(), Vector2D(1.0f, 0.0f));
  EXPECT_EQ(r1.origin(), Point2D(1.0f, 1.0f));
  EXPECT_EQ(r1.direction(), v1);

  Ray2D r4 = std::move(r2);
  v1 = { 2.0f, 0.0f };
  v1.Normalize();
  std::cout << r2.direction().x() << " " << r2.direction().y() << std::endl;
  EXPECT_EQ(r4.origin(), Point2D(5.0f, 5.0f));
  EXPECT_EQ(r4.direction(), v1);
  EXPECT_EQ(r2.origin(), Point2D(0.0f, 0.0f));
  EXPECT_EQ(r2.direction(), Vector2D(1.0f, 0.0f));
}

TEST(ray2d_test, test_intersection)
{
  //Проверка дефолтного вектора на пересечение прямоугольника, "нанизанного" на ось ОХ
  Ray2D r1 = Ray2D();
  EXPECT_EQ(r1.IsRayIntersectingBox(Box2D(Point2D(3.0f, -1.0f), Point2D(6.0f, 1.0f))), true);

  //Проверка вектора, направленного строго вниз, на пересечение того же прямоугольнкиа
  Ray2D r2 = Ray2D(Point2D(5.0f, 9.0f), Vector2D(0.0f, -1.0f));
  EXPECT_EQ(r2.IsRayIntersectingBox(Box2D(Point2D(3.0f, -1.0f), Point2D(6.0f, 1.0f))), true);

  //Проверка вектора, повернутого на 45 градусов относительно оси ОХ, на пересечение прямоугольника, лежащего по его направлению
  Ray2D r3 = Ray2D(Point2D(5.0f, 9.0f), Vector2D(1.0f, 1.0f));
  EXPECT_EQ(r3.IsRayIntersectingBox(Box2D(Point2D(7.0f, 11.0f), Point2D(10.0f, 13.0f))), true);

  //Проверка вектора, лежащего в диаметрально другом направлении, на непересечение того же прямоугольника
  Ray2D  r4 = Ray2D(Point2D(5.0f, 9.0f), Vector2D(-1.0f, -1.0f));
  EXPECT_EQ(r4.IsRayIntersectingBox(Box2D(Point2D(7.0f, 11.0f), Point2D(10.0f, 13.0f))), false);

  //Проверка вектора, направленного строго вверх, на непересечение прямоугольнкиа из пункта 1
  Ray2D r5 = Ray2D(Point2D(5.0f, 9.0f), Vector2D(0.0f, 1.0f));
  EXPECT_EQ(r5.IsRayIntersectingBox(Box2D(Point2D(3.0f, -1.0f), Point2D(6.0f, 1.0f))), false);
}
