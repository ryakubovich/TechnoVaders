#include "gtest/gtest.h"
#include "ray2d.hpp"
#include <iostream>

using namespace std;

TEST(ray2d_test, test_contruction)
{
  // Default constructor check
  Ray2D r1;
  EXPECT_EQ(r1, Ray2D(Point2D(0.0f, 0.0f),
                      Vector2D(1.0f, 0.0f)));

  // Checks for direction normalization in constructor
  Ray2D r2 = { Point2D(5.0f, 5.0f),
               Vector2D(2.0f, 0.0f) };
  EXPECT_EQ(r2, Ray2D(Point2D(5.0f, 5.0f),
                      Vector2D(1.0f, 0.0f)));

  // Checks for move (assigning) correctness
  Ray2D r3 = { Point2D(1.0f, 1.0f),
               Vector2D(1.0f, 1.0f) };
  r3 = std::move(r1);
  Vector2D v1 = { 1.0f, 1.0f };
  v1.Normalize();
  EXPECT_EQ(r3.GetOrigin(), Point2D(0.0f, 0.0f));
  EXPECT_EQ(r3.GetDirection(), Vector2D(1.0f, 0.0f));
  EXPECT_EQ(r1.GetOrigin(), Point2D(1.0f, 1.0f));
  EXPECT_EQ(r1.GetDirection(), v1);

  // Checks for move (copying) correctness
  Ray2D r4 = std::move(r2);
  v1 = { 2.0f, 0.0f };
  v1.Normalize();
  EXPECT_EQ(r4.GetOrigin(), Point2D(5.0f, 5.0f));
  EXPECT_EQ(r4.GetDirection(), v1);
  EXPECT_EQ(r2.GetOrigin(), Point2D(0.0f, 0.0f));
  EXPECT_EQ(r2.GetDirection(), Vector2D(1.0f, 0.0f));
}

TEST(ray2d_test, test_intersection)
{
  // Checks for intersection of default vector and box strung on OX axis
  Ray2D r1 = Ray2D();
  EXPECT_EQ(r1.IsRayIntersectingBox(Box2D(Point2D(3.0f, -1.0f), Point2D(6.0f, 1.0f))), true);

  // Checks for intersection of directed down vector and box from first check
  Ray2D r2 = Ray2D(Point2D(5.0f, 9.0f), Vector2D(0.0f, -1.0f));
  EXPECT_EQ(r2.IsRayIntersectingBox(Box2D(Point2D(3.0f, -1.0f), Point2D(6.0f, 1.0f))), true);

  // Checks for intersection of directed on north-east (45 degrees) vector and box laying on vector's way
  Ray2D r3 = Ray2D(Point2D(5.0f, 9.0f), Vector2D(1.0f, 1.0f));
  EXPECT_EQ(r3.IsRayIntersectingBox(Box2D(Point2D(7.0f, 11.0f), Point2D(10.0f, 13.0f))), true);

  // Checks for non-intersection of directed on south-west (225 degrees) vector and box from the previous (third) check
  Ray2D  r4 = Ray2D(Point2D(5.0f, 9.0f), Vector2D(-1.0f, -1.0f));
  EXPECT_EQ(r4.IsRayIntersectingBox(Box2D(Point2D(7.0f, 11.0f), Point2D(10.0f, 13.0f))), false);

  // Checks for non-intersection of directed up vector and box from first check
  Ray2D r5 = Ray2D(Point2D(5.0f, 9.0f), Vector2D(0.0f, 1.0f));
  EXPECT_EQ(r5.IsRayIntersectingBox(Box2D(Point2D(3.0f, -1.0f), Point2D(6.0f, 1.0f))), false);
}
