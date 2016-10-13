#include "gtest/gtest.h"
#include "box2d.hpp"
#include "point2d.hpp"
#include <iostream>

using namespace std;

TEST(box2d_test, test_construction)
{
  // Default initialization check
  Box2D box1;
  EXPECT_EQ(box1, Box2D(Point2D(0.0f, 0.0f), Point2D(0.0f, 0.0f)));

  // Checks for equality of constructors with Point2Ds and floats
  Box2D box2 = Box2D(1.0f, 3.0f, 5.0f, 6.0f);
  EXPECT_EQ(box2, Box2D(Point2D(1.0f, 3.0f), Point2D(5.0f, 6.0f)));

  // Checks for wrong points order fix
  Point2D p1 = { 4.0f, 5.0f };
  Point2D p2 = { 3.0f, 1.0f };
  Box2D box3 = Box2D(p1, p2);
  EXPECT_EQ(box3, Box2D(Point2D(3.0f, 1.0f), Point2D(4.0f, 5.0f)));
}

TEST(box2d_test, test_scale)
{
  // Checks for immovability of the box center after scaling
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
  // Checks for intersection of two default boxes
  Box2D box1 = Box2D();
  EXPECT_EQ(box1.IsBoxIntersectingBox(Box2D()), true);

  // Checks for intersection of two criss-cross boxes
  Box2D box2 = Box2D(4.0f, 1.0f, 5.0f, 5.0f);
  EXPECT_EQ(box2.IsBoxIntersectingBox(Box2D(1.0f, 2.0f, 7.0f, 3.0f)), true);

  // Checks for intersection of two nested boxes
  EXPECT_EQ(box2.IsBoxIntersectingBox(Box2D(4.3f, 2.0f, 4.7f, 4.0f)), true);

  // Checks for intersection after scaling (descreasing)
  Box2D box3 = {Point2D(6.0f, 2.0f), Point2D(8.0f, 10.0f)};
  Box2D box4 = Box2D(0.0f, 0.0f, 8.0f, 3.0f);
  EXPECT_EQ(box3.IsBoxIntersectingBox(box4), true);
  box3 /= 3.0f;
  EXPECT_EQ(box3.IsBoxIntersectingBox(box4), false);

  // Checks for intersection after scaling (increasing)
  box3 *= 3.0f;
  EXPECT_EQ(box3.IsBoxIntersectingBox(box4), true);
}
