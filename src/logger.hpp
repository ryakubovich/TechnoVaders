#pragma once

#include <fstream>
#include <iostream>
#include "singleton.hpp"

std::string const logFileName = "data/log.txt";

class Logger : public Singleton<Logger>
{
public:
  template<typename T> Logger & operator << (T const & obj)
  {
    if (!m_logFile.is_open())
      m_logFile.open(logFileName);
    m_logFile << obj << std::endl;
    return *this;
  }

  template<typename T, template<typename, typename...> class C, typename... Args>
  Logger & operator << (C<T, Args...> const & objs)
  {
    if (!m_logFile.is_open())
      m_logFile.open(logFileName);
    for (auto const & obj : objs)
      m_logFile << obj << std::endl;
    return *this;
  }

  Logger & operator << (std::string string)
  {
    if (!m_logFile.is_open())
      m_logFile.open(logFileName);
    m_logFile << string.c_str() << std::endl;
    return *this;
  }

  Logger & operator << (std::ostream & (*manip)(std::ostream &))
  {
    manip(m_logFile);
    return *this;
  }

  bool Trunc()
  {
    if (m_logFile.is_open())
      m_logFile.close();
    m_logFile.open(logFileName);
    return m_logFile.is_open();
  }

private:
  friend class Singleton<Logger>;
  Logger() = default;
  std::ofstream m_logFile;
};
