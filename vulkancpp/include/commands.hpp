#pragma once

#include <vulkan/vulkan.hpp>
#include <vector>
#include "queuefamilies.hpp"
#include "frame.hpp"

struct commandBufferIn
{
  vk::Device device;
  vk::CommandPool commandPool;
  std::vector<SwapChainFrame>& frames;
};

vk::CommandPool createCommandPool(vk::Device device, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, const bool& debug);
vk::CommandBuffer createCommandBuffer(const commandBufferIn& in, const bool& debug);
