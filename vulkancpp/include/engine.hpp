#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include <vector>
#include <iostream>
#include <stdexcept>

#include "logging.hpp"
#include "device.hpp"

class Engine
{
  public:
    Engine();
    ~Engine();
  
  private:
    bool debugMode{true};

    int width{640};
    int height{480};
    const char* title{"Engine"};
    GLFWwindow* window{nullptr};

    // Instance
    vk::Instance instance{nullptr};
    vk::DebugUtilsMessengerEXT debugMessenger{nullptr};
    vk::DispatchLoaderDynamic dispatchLoader;
    vk::SurfaceKHR surface{nullptr};

    // Device
    vk::PhysicalDevice physicalDevice{nullptr};
    vk::Device device{nullptr};
    vk::Queue graphicsQueue{nullptr};
    vk::Queue presentQueue{nullptr};

    void buildGLFWWindow();
    bool supported(std::vector<const char*>& extensions, std::vector<const char*>& layers);
    void makeInstance();
    void enableLogging();

    void makeDevice();
};