#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <cassert>

#include "device.hpp"

struct PipelineConfigInfo{
  VkViewport viewport;
  VkRect2D scissor;
  VkPipelineViewportStateCreateInfo viewportInfo;
  VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
  VkPipelineRasterizationStateCreateInfo rasterizationInfo;
  VkPipelineMultisampleStateCreateInfo multisampleInfo;
  VkPipelineColorBlendAttachmentState colorBlendAttachment;
  VkPipelineColorBlendStateCreateInfo colorBlendInfo;
  VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
  VkPipelineLayout pipelineLayout = nullptr;
  VkRenderPass renderPass = nullptr;
  uint32_t subpass = 0;
};

class Pipeline{
  public:
    Pipeline(Device& device, const std::string& vertFile, 
            const std::string& fragFile, const PipelineConfigInfo& config);
    ~Pipeline();

    // Deleted
    Pipeline(const Pipeline&) = delete;
    Pipeline& operator=(const Pipeline&) = delete;
    
    static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);
  private:
    static std::vector<char> readFile(const std::string& filename);
    void createGraphicsPipeline(const std::string& vertFile, 
                                const std::string& fragFile, const PipelineConfigInfo& config);
    void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

    Device& device;
    VkPipeline graphicsPipeline;
    VkShaderModule vertShaderModule;
    VkShaderModule frahShaderModule;
};