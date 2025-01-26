#pragma once

#include <vulkan/vulkan.hpp>
#include <vector>
#include <iostream>
#include <stdexcept>

#include "frame.hpp"

struct FrameBufferIn
{
  vk::Device device;
  vk::RenderPass renderPass;
  vk::Extent2D extent;
};

void createFrameBuffers(const FrameBufferIn& in, std::vector<SwapChainFrame>& frames, const bool& debug);
