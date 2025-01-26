#include "queuefamilies.hpp"

QueueFamilyIndices findQueueFamilies(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface, bool debug)
{
  QueueFamilyIndices indices;
  std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();
  if(debug)
  {
    std::cout << "Queue families: \n";
    std::cout << "\tNumber of queue families: " << queueFamilies.size() << "\n";
  }
  int i{0};
  for(vk::QueueFamilyProperties queueFamily : queueFamilies)
  {
    if(queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
    {
      indices.graphicsFamily = i;

      if(debug)
      {
        std::cout << "\tQueue family " << i << " supports graphics\n";
        std::cout << queueFamily.queueCount << " queues\n";
      }
    }

    if(device.getSurfaceSupportKHR(i, surface))
    {
      indices.presentFamily = i;
      if(debug)
      {
        std::cout << "\tQueue family " << i << " supports presentation\n";
      }
    }

    if(indices.isComplete())
    {
      break;
    }
  }

  return indices;
}