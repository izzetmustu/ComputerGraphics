#include "device.hpp"

void logDeviceProperties(const vk::PhysicalDevice& device)
{
  VkPhysicalDeviceProperties properties = device.getProperties();
  std::cout << "Device properties: \n";
  std::cout << "\tName: " << properties.deviceName << "\n";
  std::cout << "\tDevice type:" << "\n";
  switch (properties.deviceType)
  {
  case(VK_PHYSICAL_DEVICE_TYPE_CPU):
    std::cout << "\t\tCPU\n";
    break;
  case(VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU):
    std::cout << "\t\tDiscrete GPU\n";
    break;
  case(VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU):
    std::cout << "\t\tIntegrated GPU\n";
    break;
  case(VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU):
    std::cout << "\t\tVirtual GPU\n";
    break;
  default:
    std::cout << "\t\tOther\n";
    break;
  }
}

bool checkDeviceExtensionSupport(const vk::PhysicalDevice& device, const std::vector<const char*>& requiredExtensions, const bool& debug)
{
  std::vector<vk::ExtensionProperties> supportedExtensions = device.enumerateDeviceExtensionProperties();
  std::set<std::string> requiredSet(requiredExtensions.begin(), requiredExtensions.end());
  if(debug) {std::cout << "Supported device extensions: \n";}
  for(const auto& extension : supportedExtensions)
  {
    if(debug){std::cout << "Supported device extension: " << extension.extensionName << "\n";}
    requiredSet.erase(extension.extensionName);
  }
  return requiredSet.empty();
}

bool isSuitable(const vk::PhysicalDevice& device, bool debug)
{
  if(debug){std::cout << "Checking device suitability\n";}
  
  const std::vector<const char*> requiredExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
  
  VkPhysicalDeviceProperties properties = device.getProperties();
  VkPhysicalDeviceFeatures features = device.getFeatures();

  if((properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) && checkDeviceExtensionSupport(device, requiredExtensions, debug))
  {
    if(debug)
    {
      std::cout << "Suitable device name: " << properties.deviceName << "\n";
    }
    return true;
  }

  return false;
}

vk::PhysicalDevice choosePhysicalDevice(vk::Instance& instance,bool debug)
{
  std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();
  for(const auto& device : physicalDevices)
  {
    if(debug)
    {
      logDeviceProperties(device);
    }
    if(isSuitable(device, debug))
    {
    if(debug){ std::cout << "Physical device chosen\n"; }
      return device;
    }
  }

  return nullptr;
}

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

std::pair<vk::Device,std::pair<vk::Queue,vk::Queue>> createLogicalDevice(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, const bool& debug)
{
  vk::Device device{nullptr};
  QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface, debug);
  std::vector<uint32_t> uniqueIndices;
  uniqueIndices.push_back(indices.graphicsFamily.value());
  if(indices.presentFamily.has_value() && indices.presentFamily.value() != indices.graphicsFamily.value())
  {
    uniqueIndices.push_back(indices.presentFamily.value());
  }
  float queuePriority = 1.0f;

  std::vector<vk::DeviceQueueCreateInfo> queueCreateInfo; 
  for (uint32_t queueFamilyIndex : uniqueIndices)
  {
    queueCreateInfo.push_back(vk::DeviceQueueCreateInfo(
      vk::DeviceQueueCreateFlags(),
      queueFamilyIndex,
      1,
      &queuePriority
    ));
  }
  
  vk::PhysicalDeviceFeatures features = vk::PhysicalDeviceFeatures();

  std::vector<const char*> enabledLayers;
  if(debug)
  {
    enabledLayers.push_back("VK_LAYER_KHRONOS_validation");
  }

  vk::DeviceCreateInfo deviceCreateInfo = vk::DeviceCreateInfo(
    vk::DeviceCreateFlags(),
    queueCreateInfo.size(),
    queueCreateInfo.data(),
    enabledLayers.size(),
    enabledLayers.data(),
    0,
    nullptr,
    &features
  );

  try
  {
    device = physicalDevice.createDevice(deviceCreateInfo);
    if(debug)
    {
      std::cout << "Logical device created\n";
    }
  }
  catch(vk::SystemError& e)
  {
    std::cerr << e.what() << '\n';
    throw std::runtime_error("Failed to create logical device\n");
  }
  
  return std::make_pair(device, 
            std::make_pair(device.getQueue(indices.graphicsFamily.value(), 0),device.getQueue(indices.presentFamily.value(), 0)));
}