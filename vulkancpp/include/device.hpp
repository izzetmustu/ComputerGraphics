#pragma once

#include <vulkan/vulkan.hpp>

#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <optional>
#include <utility>

#include "swapchain.hpp"
#include "queuefamilies.hpp"

void logDeviceProperties(const vk::PhysicalDevice& device);
bool checkDeviceExtensionSupport(const vk::PhysicalDevice& device, const std::vector<const char*>& requiredExtensions, const bool& debug);
bool isSuitable(const vk::PhysicalDevice& device, bool debug);
vk::PhysicalDevice choosePhysicalDevice(vk::Instance& instance,bool debug);
std::pair<vk::Device,std::pair<vk::Queue,vk::Queue>> createLogicalDevice(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, const bool& debug);
