#include "engine.hpp"
#include <memory>

int main()
{
  std::unique_ptr<Engine> graphicsEngine = std::make_unique<Engine>();
  return 0;
}