#include "device.hpp"

bool checkDeviceExtensionSupport(const vk::PhysicalDevice& device, const std::vector<const char*>& requiredExtensions, const bool& debug)
{
  std::vector<vk::ExtensionProperties> supportedExtensions = device.enumerateDeviceExtensionProperties();
  std::set<std::string> requiredSet(requiredExtensions.begin(), requiredExtensions.end());
  if(debug) {std::cout << "Supported device extensions: \n";}
  for(const auto& extension : supportedExtensions)
  {
    if(debug){std::cout << "\t" << extension.extensionName << "\n";}
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
  
  std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

  vk::PhysicalDeviceFeatures features = vk::PhysicalDeviceFeatures();

  std::vector<const char*> enabledLayers;
  if(debug)
  {
    enabledLayers.push_back("VK_LAYER_KHRONOS_validation");
  }

  vk::DeviceCreateInfo deviceCreateInfo = vk::DeviceCreateInfo(
    vk::DeviceCreateFlags(),
    queueCreateInfo.size(), queueCreateInfo.data(),
    enabledLayers.size(), enabledLayers.data(),
    deviceExtensions.size(), deviceExtensions.data(),
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

