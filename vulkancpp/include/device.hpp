#pragma once

#include <vulkan/vulkan.hpp>

#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <optional>
#include <utility>

struct QueueFamilyIndices
{
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;

  bool isComplete()
  {
    return graphicsFamily.has_value() && presentFamily.has_value();
  }
};

void logDeviceProperties(const vk::PhysicalDevice& device);
bool checkDeviceExtensionSupport(const vk::PhysicalDevice& device, const std::vector<const char*>& requiredExtensions, const bool& debug);
bool isSuitable(const vk::PhysicalDevice& device, bool debug);
vk::PhysicalDevice choosePhysicalDevice(vk::Instance& instance,bool debug);
QueueFamilyIndices findQueueFamilies(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface, bool debug);
std::pair<vk::Device,std::pair<vk::Queue,vk::Queue>> createLogicalDevice(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, const bool& debug);