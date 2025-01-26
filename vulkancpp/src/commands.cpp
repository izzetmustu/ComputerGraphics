#include "commands.hpp"

vk::CommandPool createCommandPool(vk::Device device, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, const bool& debug)
{
  QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice, surface, debug);
  vk::CommandPoolCreateInfo poolInfo = {};
  poolInfo.flags = vk::CommandPoolCreateFlags() | vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
  poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

  try
  {
    return device.createCommandPool(poolInfo);
  }
  catch(vk::SystemError& e)
  {
    std::cerr << e.what() << '\n';
    throw std::runtime_error("Failed to create command pool\n");
  }
  
}

vk::CommandBuffer createCommandBuffer(const commandBufferIn& in, const bool& debug)
{
  vk::CommandBufferAllocateInfo allocInfo = {};
  allocInfo.commandPool = in.commandPool;
  allocInfo.level = vk::CommandBufferLevel::ePrimary;
  allocInfo.commandBufferCount = static_cast<uint32_t>(in.frames.size());

  for(int i = 0; i < in.frames.size(); i++)
  {
    try
    {
      in.frames[i].commandBuffer = in.device.allocateCommandBuffers(allocInfo)[0];
      if(debug)
      {
        std::cout << "Command buffer allocated " + std::to_string(i) + "\n";
      }
    }
    catch(vk::SystemError& e)
    {
      std::cerr << e.what() << '\n';
      throw std::runtime_error("Failed to allocate command buffer\n");
    }
  }

  try
  {
    vk::CommandBuffer commandBuffer = in.device.allocateCommandBuffers(allocInfo)[0];
    if(debug)
    {
      std::cout << "Main command buffer allocated\n";
    }
    return commandBuffer;
  }
  catch(vk::SystemError& e)
  {
    std::cerr << e.what() << '\n';
    throw std::runtime_error("Failed to allocate main command buffer\n");
  } 
}
