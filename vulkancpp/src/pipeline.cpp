#include "pipeline.hpp"

vk::PipelineLayout createPipelineLayout(const vk::Device& device, const bool& debug)
{
  vk::PipelineLayoutCreateInfo pipelineLayoutInfo = {};
  pipelineLayoutInfo.flags = vk::PipelineLayoutCreateFlags();
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pushConstantRangeCount = 0;
  
  try
  {
    return device.createPipelineLayout(pipelineLayoutInfo);
  }
  catch(vk::SystemError& e)
  {
    std::cerr << e.what() << '\n';
    throw std::runtime_error("Failed to create pipeline layout\n");
  }
}

vk::RenderPass createRenderPass(const vk::Device& device, const vk::Format& swapChainImageFormat, const bool& debug)
{
  vk::AttachmentDescription colorAttachment = {};
  colorAttachment.flags = vk::AttachmentDescriptionFlags();
  colorAttachment.format = swapChainImageFormat;
  colorAttachment.samples = vk::SampleCountFlagBits::e1;
  colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
  colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
  colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
  colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
  colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
  colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

  vk::AttachmentReference colorAttachmentRef = {};
  colorAttachmentRef.attachment = 0;
  colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

  vk::SubpassDescription subpass = {};
  subpass.flags = vk::SubpassDescriptionFlags();
  subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colorAttachmentRef;

  vk::RenderPassCreateInfo renderPassInfo = {};
  renderPassInfo.flags = vk::RenderPassCreateFlags();
  renderPassInfo.attachmentCount = 1;
  renderPassInfo.pAttachments = &colorAttachment;
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpass;

  try
  {
    return device.createRenderPass(renderPassInfo);
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    throw std::runtime_error("Failed to create render pass\n");
  }
}

GraphicsPipelineOut createGraphicsPipeline(const GraphicsPipelineIn& in, const bool& debug)
{
  vk::GraphicsPipelineCreateInfo pipelineInfo = {};
  pipelineInfo.flags = vk::PipelineCreateFlags();

  // Vertex input
  vk::PipelineVertexInputStateCreateInfo vertexInputInfo = {};
  vertexInputInfo.flags = vk::PipelineVertexInputStateCreateFlags();
  vertexInputInfo.vertexAttributeDescriptionCount = 0;
  vertexInputInfo.vertexBindingDescriptionCount = 0;
  pipelineInfo.pVertexInputState = &vertexInputInfo;
  // Input assembly
  vk::PipelineInputAssemblyStateCreateInfo inputAssembly = {};
  inputAssembly.flags = vk::PipelineInputAssemblyStateCreateFlags();
  inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;
  pipelineInfo.pInputAssemblyState = &inputAssembly;
  
  std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;
  // Vertex shader
  vk::ShaderModule vertShaderModule = createShaderModule(in.vertexFilePath, in.device, debug);
  vk::PipelineShaderStageCreateInfo vertShaderStageInfo = {};
  vertShaderStageInfo.flags = vk::PipelineShaderStageCreateFlags();
  vertShaderStageInfo.stage = vk::ShaderStageFlagBits::eVertex;
  vertShaderStageInfo.module = vertShaderModule;
  vertShaderStageInfo.pName = "main";
  shaderStages.push_back(vertShaderStageInfo);

  // Viewport
  vk::Viewport viewport = {};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(in.extent.width);
  viewport.height = static_cast<float>(in.extent.height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  // Scissor
  vk::Rect2D scissor = {};
  scissor.offset = vk::Offset2D(0, 0);
  scissor.extent = in.extent;
  vk::PipelineViewportStateCreateInfo viewportState = {};
  viewportState.flags = vk::PipelineViewportStateCreateFlags();
  viewportState.viewportCount = 1;
  viewportState.pViewports = &viewport;
  viewportState.scissorCount = 1;
  viewportState.pScissors = &scissor;
  viewportState.pNext = VK_NULL_HANDLE;
  pipelineInfo.pViewportState = &viewportState;

  // Rasterizer
  vk::PipelineRasterizationStateCreateInfo rasterizer = {};
  rasterizer.flags = vk::PipelineRasterizationStateCreateFlags();
  rasterizer.depthClampEnable = VK_FALSE;
  rasterizer.rasterizerDiscardEnable = VK_FALSE;
  rasterizer.polygonMode = vk::PolygonMode::eFill;
  rasterizer.lineWidth = 1.0f;
  rasterizer.cullMode = vk::CullModeFlagBits::eBack;
  rasterizer.frontFace = vk::FrontFace::eClockwise;
  rasterizer.depthBiasEnable = VK_FALSE;
  pipelineInfo.pRasterizationState = &rasterizer;

  // Fragment shader
  vk::ShaderModule fragmentShaderModule = createShaderModule(in.fragmentFilePath, in.device, debug);
  vk::PipelineShaderStageCreateInfo fragmentShaderStageInfo = {};
  fragmentShaderStageInfo.flags = vk::PipelineShaderStageCreateFlags();
  fragmentShaderStageInfo.stage = vk::ShaderStageFlagBits::eFragment;
  fragmentShaderStageInfo.module = fragmentShaderModule;
  fragmentShaderStageInfo.pName = "main";
  shaderStages.push_back(fragmentShaderStageInfo);
  pipelineInfo.stageCount = shaderStages.size();
  pipelineInfo.pStages = shaderStages.data();

  // Multisampling
  vk::PipelineMultisampleStateCreateInfo multisampling = {};
  multisampling.flags = vk::PipelineMultisampleStateCreateFlags();
  multisampling.sampleShadingEnable = VK_FALSE;
  multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;
  pipelineInfo.pMultisampleState = &multisampling;

  // Color blending
  vk::PipelineColorBlendAttachmentState colorBlendAttachment = {};
  colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
  colorBlendAttachment.blendEnable = VK_FALSE;
  vk::PipelineColorBlendStateCreateInfo colorBlending = {};
  colorBlending.flags = vk::PipelineColorBlendStateCreateFlags();
  colorBlending.logicOpEnable = VK_FALSE;
  colorBlending.logicOp = vk::LogicOp::eCopy;
  colorBlending.attachmentCount = 1;
  colorBlending.pAttachments = &colorBlendAttachment;
  colorBlending.blendConstants[0] = 0.0f;
  colorBlending.blendConstants[1] = 0.0f;
  colorBlending.blendConstants[2] = 0.0f;
  colorBlending.blendConstants[3] = 0.0f;
  pipelineInfo.pColorBlendState = &colorBlending;

  // Pipeline layout
  vk::PipelineLayout pipelineLayout = createPipelineLayout(in.device, debug);
  pipelineInfo.layout = pipelineLayout;

  // Render pass
  vk::RenderPass renderPass = createRenderPass(in.device, in.swapchainImageFormat, debug);
  pipelineInfo.renderPass = renderPass;

  pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

  vk::Pipeline pipeline = VK_NULL_HANDLE;
  try
  {
    pipeline = in.device.createGraphicsPipeline(VK_NULL_HANDLE, pipelineInfo).value;
    if(debug)
    {
      std::cout << "Graphics pipeline created\n";
    }
  }
  catch(vk::SystemError& e)
  {
    std::cerr << e.what() << '\n';
    throw std::runtime_error("Failed to create graphics pipeline\n");
  }

  GraphicsPipelineOut out = {};
  out.pipeline = pipeline;
  out.pipelineLayout = pipelineLayout;
  out.renderPass = renderPass;

  in.device.destroyShaderModule(vertShaderModule);
  in.device.destroyShaderModule(fragmentShaderModule);

  return out;
}