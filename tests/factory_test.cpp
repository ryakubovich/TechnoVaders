#include "factory.hpp"
#include "gtest/gtest.h"
#include "space.hpp"

TEST(factory_test, general)
{
  Factory factory;
  BulletManager bm;
  auto player = factory.Create<Player>(Box2D(), 100.0f, 1, "AK74", 30, 5.45f, 45.0f, 10.0f, 45.0f, 150.0f, bm);
  EXPECT_EQ(player->GetHealth(), 100.0f);

  auto bulletManagerPointer = factory.Create<BulletManager>();
  EXPECT_EQ(bulletManagerPointer->GetPlayersBullets().size(), 0);
}
