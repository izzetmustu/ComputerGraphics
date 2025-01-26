#pragma once

#include <vulkan/vulkan.hpp>
#include <vector>
#include <iostream>
#include <stdexcept>

#include "queuefamilies.hpp"
#include "logging.hpp"
#include "frame.hpp"

struct SwapChainSupportDetails
{
  vk::SurfaceCapabilitiesKHR capabilities;
  std::vector<vk::SurfaceFormatKHR> formats;
  std::vector<vk::PresentModeKHR> presentModes;
};

struct SwapChain
{
  vk::SwapchainKHR swapChain;
  std::vector<SwapChainFrame> frames;
  vk::Format format;
  vk::Extent2D extent;
};

SwapChainSupportDetails querySwapChainSupport(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface, const bool& debug);
vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, int width, int height);
SwapChain crateSwapChain(const vk::PhysicalDevice& physicalDevice, const vk::Device& device, const vk::SurfaceKHR& surface, const int& width, const int& height, const bool& debug);
