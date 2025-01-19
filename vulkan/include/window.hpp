#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

class Window{
  private:
    GLFWwindow* window;
    const int width, height;
    std::string title;
    void initWindow();
  public:
    Window(int width, int height, std::string title);
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    ~Window();
    bool shouldClose(){return glfwWindowShouldClose(window);}
};