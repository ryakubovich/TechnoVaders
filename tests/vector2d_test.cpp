#include "gtest/gtest.h"
#include "vector2d.hpp"

TEST(vector2d_test, test_construction)
{
  Vector2D v1;
  EXPECT_EQ(v1.x(), 1.0f);
  EXPECT_EQ(v1.y(), 0.0f);

  Vector2D v2 = { 1.0f, 2.5f };
  EXPECT_EQ(v2.x(), 1.0f);
  EXPECT_EQ(v2.y(), 2.5f);

  Vector2D v3 = { 1.0f, 2.0f, 3.0f, 4.0f };
  EXPECT_EQ(v3.x(), 1.0f);
  EXPECT_EQ(v3.y(), 2.0f);

  Vector2D v4 = { 1.0f, 3.0f };
  Vector2D v5 = v4;
  EXPECT_EQ(v5.x(), 1.0f);
  EXPECT_EQ(v5.y(), 3.0f);

  Vector2D v6 = { 1.0f, 1.0f };
  Vector2D v7 = { 5.0f, 6.0f };
  v6 = std::move(v7);
  EXPECT_EQ(v6.x(), 5.0f);
  EXPECT_EQ(v6.y(), 6.0f);
  EXPECT_EQ(v7.x(), 1.0f);
  EXPECT_EQ(v7.y(), 1.0f);

  Vector2D v8 = std::move(v6);
  EXPECT_EQ(v8.x(), 5.0f);
  EXPECT_EQ(v8.y(), 6.0f);
  EXPECT_EQ(v6.x(), 1.0f);
  EXPECT_EQ(v6.y(), 0.0f);
}

TEST(vector2d_test, test_calculus)
{
  Vector2D v1 = { 3.0f, 2.0f };
  Vector2D v2 = { 1.0f, 3.0f };
  EXPECT_EQ(v1 + v2, Vector2D(4.0f, 5.0f));
  EXPECT_EQ(v1 - v2, Vector2D(2.0f, -1.0f));
  EXPECT_EQ(v1 * 5.0f, Vector2D(15.0f, 10.0f));

  v1 += { 2.0f, 5.0f };
  EXPECT_EQ(v1, Vector2D(5.0f, 7.0f));

  v1 -= { 2.0f, 4.0f };
  EXPECT_EQ(v1, Vector2D(3.0f, 3.0f));

  v1 *= 2.0f;
  EXPECT_EQ(v1, Vector2D(6.0f, 6.0f));

  v1 /= 0.0f;
  EXPECT_EQ(v1, Vector2D(6.0f, 6.0f));

  v1 /= 2.0f;
  EXPECT_EQ(v1, Vector2D(3.0f, 3.0f));
}

TEST(vector2d_test, test_vector_operations)
{
  Vector2D v1 = { 2.0f, 0.0f };
  v1.Normalize();
  EXPECT_EQ(v1, Vector2D(1.0f, 0.0f));

  Vector2D v2 = { 0.0f, 1.0f };
  Vector2D v3 = { 1.0f, 0.0f };
  EXPECT_EQ(v2.Scalar(v3), 0.0f);
  EXPECT_EQ(v2.Determinant(v3), -1.0f);
}

TEST(vector2d_test, test_logical)
{
  Vector2D v1 = { 1.0f, 5.0f };
  Vector2D v2 = { 1.0f, 5.0f };
  Vector2D v3 = { 0.0f, 3.0f };
  EXPECT_EQ(v1, v2);
  EXPECT_NE(v1, v3);
}
