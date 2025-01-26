#include "engine.hpp"

Engine::Engine(int width, int height, const char* title, GLFWwindow* window, bool debug) : width(width), height(height), title(title), window(window), debugMode(debug)
{
  makeInstance();
  if(debugMode)
  {
    enableLogging();
  }
  VkSurfaceKHR c_surface;
  if(glfwCreateWindowSurface(instance, window, nullptr, &c_surface) != VK_SUCCESS)
  {
    throw std::runtime_error("Failed to create window surface\n");
  }
  else
  {
    if(debugMode)
    {
      std::cout << "Window surface created\n";
    }
  }
  surface = c_surface;

  makeDevice();
  makePipeline();
  finishSetup();
}

bool Engine::supported(std::vector<const char*>& extensions, std::vector<const char*>& layers)
{
  std::vector<vk::ExtensionProperties> supportedExtensions = vk::enumerateInstanceExtensionProperties();
  std::set<std::string> requiredSet(extensions.begin(), extensions.end());

  for (auto &supported : supportedExtensions)
  {
    if(debugMode)
    {
      std::cout << "Supported extension: " << supported.extensionName << "\n";
    }
    requiredSet.erase(supported.extensionName);
  }

  // If anything is left in requiredSet, it’s not supported
  if (!requiredSet.empty())
  {
    for (auto &ext : requiredSet)
    {
        std::cerr << "Required extension not supported: " << ext << "\n";
    }
    return false;
  }

  std::vector<vk::LayerProperties> supportedLayers = vk::enumerateInstanceLayerProperties();
  std::set<std::string> requiredLayerSet(layers.begin(), layers.end());

  for(auto& supported : supportedLayers)
  {
    if(debugMode)
    {
      std::cout << "Supported layer: " << supported.layerName << "\n";
    }
    requiredLayerSet.erase(supported.layerName);
  }
  if(!requiredLayerSet.empty())
  {
    for(auto& layer : requiredLayerSet)
    {
      std::cerr << "Required layer not supported: " << layer << "\n";
    }
    return false;
  }
 
  return true;
}

void Engine::makeInstance()
{
  uint32_t version{0};
  vkEnumerateInstanceVersion(&version);
  if(debugMode)
  {
    std::cout << "System suppport vulkan Variant " << VK_API_VERSION_VARIANT(version)
            << ", Major: " << VK_API_VERSION_MAJOR(version)
            << ", Minor: " << VK_API_VERSION_MINOR(version)
            << ", Patch: " << VK_API_VERSION_PATCH(version) << "\n";
  }
  
  // zero out patch
  version &= ~(0xFFFU);
  // or use api
  version = VK_MAKE_API_VERSION(0,1,0,0);

  vk::ApplicationInfo appInfo = vk::ApplicationInfo(
    title,
    version,
    "Engine",
    version,
    version
  );

  uint32_t glfwExtensionCount {0};
  const char** glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

  if(debugMode)
  {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    std::cout << "Required extensions: \n";
    for(const auto& extension : extensions)
    {
      std::cout << "\t" << extension << "\n";
    }
  }

  std::vector<const char*> layers;
  if(debugMode)
  {
    layers.push_back("VK_LAYER_KHRONOS_validation");
  }

  if(!supported(extensions, layers))
  {
    throw std::runtime_error("Required extensions not supported\n");
  }

  vk::InstanceCreateInfo createInfo(
            vk::InstanceCreateFlags(),
            &appInfo,
            static_cast<uint32_t>(layers.size()), layers.data(),
            static_cast<uint32_t>(extensions.size()), extensions.data()
  );

  try
  {
    instance = vk::createInstance(createInfo, nullptr);
    dispatchLoader = vk::DispatchLoaderDynamic(instance, vkGetInstanceProcAddr);
  }
  catch(vk::SystemError err)
  {
    std::cerr << err.what() << '\n';
    throw std::runtime_error("Failed to create instance\n");
  }
  
}

void Engine::enableLogging()
{
  debugMessenger = makeDebugMessenger(instance, dispatchLoader);
  std::cout << "Debug messenger created\n";
}

void Engine::makeDevice()
{
  physicalDevice = choosePhysicalDevice(instance, debugMode);
  if(physicalDevice == VK_NULL_HANDLE)
  {
    throw std::runtime_error("Failed to find a suitable GPU\n");
  }
  std::pair<vk::Device, std::pair<vk::Queue, vk::Queue>> result = createLogicalDevice(physicalDevice, surface, debugMode);
  device = result.first;
  graphicsQueue = result.second.first;
  presentQueue = result.second.second;
  
  SwapChain bundle = crateSwapChain(physicalDevice, device, surface, width, height, debugMode);
  swapchain = bundle.swapChain;
  swapchainFrames = bundle.frames;
  swapchainImageFormat = bundle.format;
  swapchainExtent = bundle.extent;
}

void Engine::makePipeline()
{
  GraphicsPipelineIn in = {};
  in.device = device;
  in.vertexFilePath = "build/shaders/vert.spv";
  in.fragmentFilePath = "build/shaders/frag.spv";
  in.swapchainImageFormat = swapchainImageFormat;
  in.extent = swapchainExtent;
  GraphicsPipelineOut out = createGraphicsPipeline(in, debugMode);
  pipelineLayout = out.pipelineLayout;
  renderPass = out.renderPass;
  pipeline = out.pipeline;
}

void Engine::finishSetup()
{
  FrameBufferIn fbIn = {};
  fbIn.device = device;
  fbIn.renderPass = renderPass;
  fbIn.extent = swapchainExtent;
  createFrameBuffers(fbIn, swapchainFrames, debugMode);
  
  commandPool = createCommandPool(device, physicalDevice, surface, debugMode);
  if(debugMode)
  {
    std::cout << "Engine setup complete\n";
  }

  commandBufferIn cbIn = {device, commandPool, swapchainFrames};
  mainCommandBuffer = createCommandBuffer(cbIn, debugMode);

  inFlightFence = createFence(device, debugMode);
  imageAvailableSemaphore = createSemaphore(device, debugMode);
  renderFinishedSemaphore = createSemaphore(device, debugMode);
}

void Engine::recordDrawCommands(vk::CommandBuffer commandBuffer, uint32_t imageIndex)
{
  vk::CommandBufferBeginInfo beginInfo = {};
//   beginInfo.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse;
  try
  {
    commandBuffer.begin(beginInfo);
  }
  catch(vk::SystemError& e)
  {
    std::cerr << e.what() << '\n';
  }
  
  vk::RenderPassBeginInfo renderPassInfo = {};
  renderPassInfo.renderPass = renderPass;
  renderPassInfo.framebuffer = swapchainFrames[imageIndex].framebuffer;
  renderPassInfo.renderArea.offset = vk::Offset2D(0, 0);
  renderPassInfo.renderArea.extent = swapchainExtent;
  vk::ClearValue clearColor = vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
  renderPassInfo.clearValueCount = 1;
  renderPassInfo.pClearValues = &clearColor;

  commandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
  commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);
  commandBuffer.draw(3, 1, 0, 0);
  commandBuffer.endRenderPass();

  try
  {
    commandBuffer.end();
  }
  catch(vk::SystemError& e)
  {
    std::cerr << e.what() << '\n';
  }

}

void Engine::render()
{
  device.waitForFences(1, &inFlightFence, VK_TRUE, UINT64_MAX);
  device.resetFences(1, &inFlightFence);

  uint32_t imageIndex{device.acquireNextImageKHR(swapchain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE).value};

  vk::CommandBuffer commandBuffer = swapchainFrames[imageIndex].commandBuffer;
  commandBuffer.reset();
  recordDrawCommands(commandBuffer, imageIndex);

  vk::SubmitInfo submitInfo = {};
  vk::Semaphore waitSemaphores[] = {imageAvailableSemaphore};
  vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;
  vk::Semaphore signalSemaphores[] = {renderFinishedSemaphore};
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  try
  {
    graphicsQueue.submit(submitInfo, inFlightFence);
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  
  vk::PresentInfoKHR presentInfo = {};
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores;
  vk::SwapchainKHR swapChains[] = {swapchain};
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;
  presentInfo.pImageIndices = &imageIndex;

  presentQueue.presentKHR(presentInfo);
}

Engine::~Engine()
{
  device.waitIdle();
  if(debugMode)
  {
    std::cout << "Engine being destroyed\n";
    instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, dispatchLoader);
  }
  device.destroyFence(inFlightFence);
  device.destroySemaphore(imageAvailableSemaphore);
  device.destroySemaphore(renderFinishedSemaphore);
  device.destroyCommandPool(commandPool);
  device.destroyPipeline(pipeline);
  device.destroyPipelineLayout(pipelineLayout);
  device.destroyRenderPass(renderPass);
  for(auto& frame : swapchainFrames)
  {
    device.destroyImageView(frame.imageView);
    device.destroyFramebuffer(frame.framebuffer);
  }
  device.destroySwapchainKHR(swapchain);
  device.destroy();
  instance.destroySurfaceKHR(surface);
  instance.destroy();
  glfwTerminate();
}
