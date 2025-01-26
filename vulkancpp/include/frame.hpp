#pragma once
#include <vulkan/vulkan.hpp>

struct SwapChainFrame
{
  vk::Image image;
  vk::ImageView imageView;
  vk::Framebuffer framebuffer;
  vk::CommandBuffer commandBuffer;
};