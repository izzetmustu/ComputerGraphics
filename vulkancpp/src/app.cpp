#include "app.hpp"

App::App(int width, int height, const char* title, bool debug)
{
  buildGLFWWindow(width, height, title, debug);
  graphicsEngine = std::make_unique<Engine>(width, height, title, window, debug);
}

void App::buildGLFWWindow(int width, int height, const char* title, bool debug)
{
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  if(window = glfwCreateWindow(width, height, title, nullptr, nullptr))
  {
    if(debug)
    {
      std::cout << "GLFW window created succesfully with width: " << width << ", height: " << height << "\n";
    }
  }
  else 
  {
    throw std::runtime_error("Failed to create GLFW window\n");
  }
}

void App::run()
{
  while(!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
    graphicsEngine->render();
    calculateFrameRate();
  }
}

void App::calculateFrameRate()
{
  currentTime = glfwGetTime();
  double delta = currentTime - lastTime;

  if(delta >= 1.0)
  {
    int frameRate {std::max(1, int(numFrames/delta))};
    std::stringstream title;
    title << "Frame rate: " << frameRate;
    glfwSetWindowTitle(window, title.str().c_str());
    lastTime = currentTime;
    numFrames = -1;
    frameTime = float(1000.0/frameRate);
  }
  ++numFrames;
}

App::~App()
{
  glfwDestroyWindow(window);
  glfwTerminate();
}