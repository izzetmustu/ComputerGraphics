#include "sync.hpp"

vk::Semaphore createSemaphore(const vk::Device& device, const bool& debug)
{
  vk::SemaphoreCreateInfo semaphoreInfo = {};
  semaphoreInfo.flags = vk::SemaphoreCreateFlags();
  try
  {
    return device.createSemaphore(semaphoreInfo);
  }
  catch(vk::SystemError& e)
  {
    std::cerr << e.what() << '\n';
    throw std::runtime_error("Failed to create semaphore\n");
  }
}

vk::Fence createFence(const vk::Device& device, const bool& debug)
{
  vk::FenceCreateInfo fenceInfo = {};
  fenceInfo.flags = vk::FenceCreateFlags() | vk::FenceCreateFlagBits::eSignaled;
  try
  {
    return device.createFence(fenceInfo);
  }
  catch(vk::SystemError& e)
  {
    std::cerr << e.what() << '\n';
    throw std::runtime_error("Failed to create fence\n");
  }
}