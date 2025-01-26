#include <memory>

#include "app.hpp"

int main()
{
  std::unique_ptr<App> app = std::make_unique<App>(800, 600, "Vulkan App", true);
  app->run();
  
  return 0;
}