#include "app.hpp"

void App::run(){
  while(!window.shouldClose()){
    glfwPollEvents();
  }
}

void App::createPipelineLayout(){
  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 0;
  pipelineLayoutInfo.pPushConstantRanges = nullptr;

if(vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS){
  throw std::runtime_error("failed to create pipeline layout!");
}
}

void App::createPipeline(){
  auto pipelineConfig = Pipeline::defaultPipelineConfigInfo(swapChain.width(), swapChain.height());
  pipelineConfig.renderPass = swapChain.getRenderPass();
  pipelineConfig.pipelineLayout = pipelineLayout;
  pipeline = std::make_unique<Pipeline>(device, "build/shaders/vert.spv", "build/shaders/frag.spv", pipelineConfig);    
}

void App::createCommandBuffers(){

}

void App::drawFrame(){

}

App::App(std::string title) : window{WIDTH, HEIGHT, title}, device{window},
                            swapChain{device, window.getExtent()} {
  createPipelineLayout();
  createPipeline();
  createCommandBuffers();
};

App::~App(){
  vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
}