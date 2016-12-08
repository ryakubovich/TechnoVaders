#include "gtest/gtest.h"
#include "space.hpp"
#include <iostream>

TEST(space_test, space_script_test)
{
  Space space(100.0f, 5, "Some name 1", 30, 105.0f, 1.0f, 200.0f, 1.0f, 180.0f, 3, 100.0f, "Some name 2", 30, 50.0f, 1.0f);
  float elapsedSeconds = 1.0f;
  int currentSize = space.GetAI().GetAliens().size();
  while (space.GetPlayer().GetBox().GetCenter().x() <= space.GetAI().GetAliens().begin()->GetBox().GetCenter().x())
    space.InputProcessing(InputType::MoveRight);
  space.InputProcessing(InputType::Shot);
  Logger::Instance() << "Killing of alien 1" << std::endl;
  Logger::Instance() << "Before killing: " << space.GetAI() << space.GetBM() << std::endl;
  while (space.GetAI().GetAliens().size() == currentSize)
    space.Update(elapsedSeconds);
  EXPECT_EQ(space.GetPlayer().GetBox(), Box2D(3.0f, 1.0f, 8.0f, 6.0f));
  EXPECT_EQ(space.GetPlayer().GetScore(), 1);
  EXPECT_EQ(space.GetPlayer().GetGun().GetScore(), 100.0f);
  EXPECT_EQ(space.GetBM().GetPlayersBullets().size(), 0);
  EXPECT_EQ(space.GetAI().GetAliens().size(), 2);

  while (space.GetPlayer().GetBox().GetCenter().x() <= space.GetAI().GetAliens().begin()->GetBox().GetCenter().x())
    space.InputProcessing(InputType::MoveRight);
  space.InputProcessing(InputType::Shot);
  currentSize--;
  Logger::Instance() << "Killing of alien 2" << std::endl;
  Logger::Instance() << "Before killing: " << space.GetAI() << space.GetBM() << std::endl;
  while (space.GetAI().GetAliens().size() == currentSize)
    space.Update(elapsedSeconds);
  EXPECT_EQ(space.GetAI().GetAliens().size(), 1);
  EXPECT_EQ(space.GetBM().GetPlayersBullets().size(), 0);

  space.InputProcessing(InputType::LaunchMissile);
  EXPECT_EQ(space.GetPlayer().GetGun().GetScore(), 20.0f);
  while (space.GetBM().GetPlayersMissiles().begin()->GetBox().GetCenter().x() < space.GetAI().GetAliens().begin()->GetBox().GetCenter().x())
    space.InputProcessing(InputType::MoveMissileRight);
  currentSize--;
  Logger::Instance() << "Killing of alien 3" << std::endl;
  Logger::Instance() << "Before killing: " << space.GetAI() << space.GetBM() << std::endl;
  while (space.GetAI().GetAliens().size() == currentSize)
    space.Update(elapsedSeconds);
  EXPECT_EQ(space.GetPlayer().GetScore(), 3);
  EXPECT_EQ(space.GetPlayer().GetGun().GetScore(), 120.0f);
  EXPECT_EQ(space.GetAI().GetAliens().size(), 0);
  EXPECT_EQ(space.GetBM().GetPlayersMissiles().size(), 0);
}
