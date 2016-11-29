#pragma once

#include <fstream>
#include "singleton.hpp"

class Logger : public Singleton<Logger>
{
public:
  template<typename T> Logger & operator << (T const & obj)
  {
    if (!m_logFile.is_open())
      m_logFile.open("data/log.txt");
    m_logFile << obj << std::endl;
    return *this;
  }

  template<typename T, template<typename, typename...> class C, typename... Args>
  Logger & operator << (C<T, Args...> const & objs)
  {
    if (!m_logFile.is_open())
      m_logFile.open("data/log.txt");
    for (auto const & obj : objs)
      m_logFile << obj << std::endl;
    return *this;
  }

  Logger & operator << (std::ostream & (*manip)(std::ostream &))
  {
    manip(m_logFile);
    return *this;
  }

private:
  friend class Singleton<Logger>;
  Logger() = default;
  std::ofstream m_logFile;
};
