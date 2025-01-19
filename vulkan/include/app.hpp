#pragma once

#include "window.hpp"
#include "pipeline.hpp"
#include "device.hpp"

class App{
  private:
    Window window;
    Device device;
    Pipeline pipeline;
  public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;
    App(std::string title) : window{WIDTH, HEIGHT, title}, device(window) ,
                    pipeline(device, "build/shaders/vert.spv", "build/shaders/frag.spv",
                    Pipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)){};
    void run();
};