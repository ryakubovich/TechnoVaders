#include "ai.hpp"
#include "gtest/gtest.h"
#include <iostream>

TEST(ai_test, test_constructor)
{
  //First alien
  Point2D p1 = {0.0f, 10.0f};
  Point2D p2 = {4.0f, 12.0f};
  Alien alien1 = Alien(Box2D(p1, p2), 100.0f, "alien", 10, 1.0f, 50.0f, 0.0f, 0.0f, 0.0f, bm);
  EXPECT_EQ(alien1, Alien(Box2D(Point2D(0.0f, 10.0f), Point2D(4.0f, 12.0f)),
                          100.0f, "alien", 10, 1.0f, 50.0f, 0.0f, 0.0f, 0.0f, bm));
}

