#pragma once

#include <memory>

class Factory
{
public:
  template<typename T, typename... Args>
  std::unique_ptr<T> Create(Args && ... args)
  {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
  }
};
