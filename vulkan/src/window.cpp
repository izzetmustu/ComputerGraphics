#include "window.hpp"

Window::Window(int width, int height, std::string title) 
  : width{width}, height{height}, title{title} {
  initWindow();
};

void Window::initWindow(){
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
}

Window::~Window(){
  glfwDestroyWindow(window);
  glfwTerminate();
}