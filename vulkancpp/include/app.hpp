#pragma once

#include <vulkan/vulkan.hpp>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <memory>

#include "engine.hpp"

class App
{
  public:
    App(int width, int height, const char* title, bool debug);
    ~App();
  
    void run();

  private:
    std::unique_ptr<Engine> graphicsEngine;
    GLFWwindow* window;

    double lastTime, currentTime;
    int numFrames;
    float frameTime;

    void buildGLFWWindow(int width, int height, const char* title, bool debug);
    void calculateFrameRate();
};
