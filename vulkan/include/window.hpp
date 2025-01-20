#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <stdexcept>

class Window{
  private:
    GLFWwindow* window;
    const int width, height;
    std::string title;
    void initWindow();
  public:
    Window(int width, int height, std::string title);
    ~Window();
    
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    VkExtent2D getExtent(){return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};}
    bool shouldClose(){return glfwWindowShouldClose(window);}
    void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
};