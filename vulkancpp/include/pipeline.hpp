#pragma once

#include <vulkan/vulkan.hpp>
#include <vector>
#include <iostream>
#include <stdexcept>

#include "shader.hpp"

struct GraphicsPipelineIn
{
  vk::Device device;
  std::string vertexFilePath;
  std::string fragmentFilePath;
  vk::Extent2D extent;
  vk::Format swapchainImageFormat;
};

struct GraphicsPipelineOut
{
  vk::PipelineLayout pipelineLayout;
  vk::RenderPass renderPass;
  vk::Pipeline pipeline;
};

vk::PipelineLayout createPipelineLayout(const vk::Device& device, const bool& debug);
vk::RenderPass createRenderPass(const vk::Device& device, const vk::Format& swapChainImageFormat, const bool& debug);
GraphicsPipelineOut createGraphicsPipeline(const GraphicsPipelineIn& in, const bool& debug);