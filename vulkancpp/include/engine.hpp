#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include <vector>
#include <iostream>
#include <stdexcept>

#include "logging.hpp"
#include "device.hpp"
#include "frame.hpp"
#include "framebuffer.hpp"
#include "commands.hpp"
#include "sync.hpp"
#include "swapchain.hpp"
#include "pipeline.hpp"
#include "commands.hpp"

class Engine
{
  public:
    Engine(int width, int height, const char* title, GLFWwindow* window, bool debug);
    ~Engine();
  
    void render();

  private:
    bool debugMode{true};

    int width;
    int height;
    const char* title;
    GLFWwindow* window{nullptr};

    // Instance
    vk::Instance instance{VK_NULL_HANDLE};
    vk::DebugUtilsMessengerEXT debugMessenger{VK_NULL_HANDLE};
    vk::DispatchLoaderDynamic dispatchLoader;
    vk::SurfaceKHR surface{VK_NULL_HANDLE};

    // Device
    vk::PhysicalDevice physicalDevice{VK_NULL_HANDLE};
    vk::Device device{VK_NULL_HANDLE};
    vk::Queue graphicsQueue{VK_NULL_HANDLE};
    vk::Queue presentQueue{VK_NULL_HANDLE};
    vk::SwapchainKHR swapchain{VK_NULL_HANDLE};
    std::vector<SwapChainFrame> swapchainFrames;
    vk::Format swapchainImageFormat;
    vk::Extent2D swapchainExtent;

    // Pipeline
    vk::PipelineLayout pipelineLayout{VK_NULL_HANDLE};
    vk::RenderPass renderPass{VK_NULL_HANDLE};
    vk::Pipeline pipeline{VK_NULL_HANDLE};

    // Commands
    vk::CommandPool commandPool{VK_NULL_HANDLE};
    vk::CommandBuffer mainCommandBuffer{VK_NULL_HANDLE};

    // Sync
    vk::Semaphore imageAvailableSemaphore{VK_NULL_HANDLE};
    vk::Semaphore renderFinishedSemaphore{VK_NULL_HANDLE};
    vk::Fence inFlightFence{VK_NULL_HANDLE};

    bool supported(std::vector<const char*>& extensions, std::vector<const char*>& layers);
    void makeInstance();
    void enableLogging();

    void makeDevice();
    void makePipeline();
    void finishSetup();

    void recordDrawCommands(vk::CommandBuffer commandBuffer, uint32_t imageIndex);
};