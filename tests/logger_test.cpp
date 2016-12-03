#include "logger.hpp"
#include "gtest/gtest.h"
#include <sstream>

TEST(logger_test, test_construction)
{
  Logger * logger1 = &(Logger::Instance());
  Logger * logger2 = &(Logger::Instance());
  Logger * logger3 = &(Logger::Instance());
  EXPECT_EQ(logger1, logger2);
  EXPECT_EQ(logger2, logger3);
}

TEST(logger_test, test_output)
{
  Logger::Instance().Trunc();
  Logger::Instance() << "New text";
  std::stringstream def;
  def << "New text" << std::endl;
  std::ifstream file("data/log.txt");
  std::stringstream string;
  string << file.rdbuf();
  EXPECT_EQ(string.str(), def.str());
}
