#pragma once

#include "window.hpp"
#include "pipeline.hpp"

class App{
  private:
    Window window;
    Pipeline pipeline{"build/shaders/vert.spv", "build/shaders/frag.spv"};
  public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;
    App(std::string title) : window{WIDTH, HEIGHT, title}{};
    void run();
};