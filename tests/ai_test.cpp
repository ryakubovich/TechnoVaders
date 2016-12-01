#include "ai.hpp"
#include "gtest/gtest.h"

TEST(ai_test, alien_test_constructor)
{
  Point2D p1 = { 0.0f, 10.0f };
  Point2D p2 = { 4.0f, 12.0f };
  BulletManager bm;
  Alien alien1 = Alien(Box2D(p1, p2), 100.0f, "M16A1", 10, 1.0f, 50.0f, bm);
  EXPECT_EQ(alien1.GetHealth(), 100.0f);
  EXPECT_EQ(alien1.GetGun().GetLimit(), 0.0f);
  alien1.Damage(50.0f);
  EXPECT_EQ(alien1.GetHealth(), 50.0f);
}

TEST(ai_test, ai_test_constructor)
{
  BulletManager bm;
  AI ai(3, 100.0f, "M16A1", 10, 1.0f, 50.0f, bm);
  EXPECT_EQ(ai.GetAliens().size(), 3);
  auto aliens = ai.GetAliens();
  for (auto & alien : aliens)
  {
    EXPECT_EQ(alien.GetHealth(), 100.0f);
    EXPECT_EQ(alien.GetGun().GetLimit(), 0.0f);
    alien.Shot();
  }
  EXPECT_EQ(bm.GetAliensBullets().size(), 3);
}

TEST(ai_test, bm_test)
{
  BulletManager bm;
  AI ai(3, 100.0f, "M16A1", 10, 1.0f, 50.0f, bm);
  auto aliens = ai.GetAliens();
  for (auto & alien : aliens)
    alien.Shot();
  EXPECT_EQ(bm.GetAliensBullets().size(), 3);
  auto aliensBullets = bm.GetAliensBullets();
  bm.DeleteBullet(0, aliensBullets.begin());
  Logger::Instance() << bm << std::endl;
  EXPECT_EQ(bm.GetAliensBullets().size(), 2);
}

