#pragma once

#include "window.hpp"
#include "pipeline.hpp"
#include "device.hpp"
#include "swap_chain.hpp"

#include <memory>
#include <vector>
#include <stdexcept>

class App{
  private:
    Window window;
    Device device;
    SwapChain swapChain;
    std::unique_ptr<Pipeline> pipeline; 
    VkPipelineLayout pipelineLayout;
    std::vector<VkCommandBuffer> commandBuffers;

    void createPipelineLayout();
    void createPipeline();
    void createCommandBuffers();
    void drawFrame();
  public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    App(std::string title);
    ~App();
    void run();

    App(const App&) = delete;
    App& operator=(const App&) = delete;
};