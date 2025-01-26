#include "logging.hpp"

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


VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
  VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
  VkDebugUtilsMessageTypeFlagsEXT messageType,
  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
  void* pUserData) 
{
  std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
  return VK_FALSE;
}

vk::DebugUtilsMessengerEXT makeDebugMessenger(vk::Instance& instance, vk::DispatchLoaderDynamic& dldi)
{
  /*
  * DebugUtilsMessengerCreateInfoEXT( VULKAN_HPP_NAMESPACE::DebugUtilsMessengerCreateFlagsEXT flags_           = {},
  								VULKAN_HPP_NAMESPACE::DebugUtilsMessageSeverityFlagsEXT messageSeverity_ = {},
  								VULKAN_HPP_NAMESPACE::DebugUtilsMessageTypeFlagsEXT     messageType_     = {},
  								PFN_vkDebugUtilsMessengerCallbackEXT                    pfnUserCallback_ = {},
  								void * pUserData_ = {} )
  */
vk::DebugUtilsMessengerCreateInfoEXT createInfo = vk::DebugUtilsMessengerCreateInfoEXT(
  vk::DebugUtilsMessengerCreateFlagsEXT(),
  vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
  vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
  debugCallback,
  nullptr);

  return instance.createDebugUtilsMessengerEXT(createInfo, nullptr, dldi);
}

std::vector<std::string> logTransformBits(const vk::SurfaceTransformFlagsKHR& flags)
{
  std::vector<std::string> transformBits;
  if(flags & vk::SurfaceTransformFlagBitsKHR::eIdentity)
  {
    transformBits.push_back("Identity");
  }
  if(flags & vk::SurfaceTransformFlagBitsKHR::eRotate90)
  {
    transformBits.push_back("Rotate 90");
  }
  if(flags & vk::SurfaceTransformFlagBitsKHR::eRotate180)
  {
    transformBits.push_back("Rotate 180");
  }
  if(flags & vk::SurfaceTransformFlagBitsKHR::eRotate270)
  {
    transformBits.push_back("Rotate 270");
  }
  if(flags & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirror)
  {
    transformBits.push_back("Horizontal mirror");
  }
  if(flags & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate90)
  {
    transformBits.push_back("Horizontal mirror rotate 90");
  }
  if(flags & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate180)
  {
    transformBits.push_back("Horizontal mirror rotate 180");
  }
  if(flags & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate270)
  {
    transformBits.push_back("Horizontal mirror rotate 270");
  }
  if(flags & vk::SurfaceTransformFlagBitsKHR::eInherit)
  {
    transformBits.push_back("Inherit");
  }

  return transformBits;
}

std::vector<std::string> logCompositeAlphaBits(const vk::CompositeAlphaFlagsKHR& flags)
{
  std::vector<std::string> compositeAlphaBits;
  if(flags & vk::CompositeAlphaFlagBitsKHR::eOpaque)
  {
    compositeAlphaBits.push_back("Opaque");
  }
  if(flags & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied)
  {
    compositeAlphaBits.push_back("Pre-multiplied");
  }
  if(flags & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied)
  {
    compositeAlphaBits.push_back("Post-multiplied");
  }
  if(flags & vk::CompositeAlphaFlagBitsKHR::eInherit)
  {
    compositeAlphaBits.push_back("Inherit");
  }

  return compositeAlphaBits;
}

std::vector<std::string> logImageUsageBits(const vk::ImageUsageFlags& flags)
{
  std::vector<std::string> imageUsageBits;
  if(flags & vk::ImageUsageFlagBits::eTransferSrc)
  {
    imageUsageBits.push_back("Transfer source");
  }
  if(flags & vk::ImageUsageFlagBits::eTransferDst)
  {
    imageUsageBits.push_back("Transfer destination");
  }
  if(flags & vk::ImageUsageFlagBits::eSampled)
  {
    imageUsageBits.push_back("Sampled");
  }
  if(flags & vk::ImageUsageFlagBits::eStorage)
  {
    imageUsageBits.push_back("Storage");
  }
  if(flags & vk::ImageUsageFlagBits::eColorAttachment)
  {
    imageUsageBits.push_back("Color attachment");
  }
  if(flags & vk::ImageUsageFlagBits::eDepthStencilAttachment)
  {
    imageUsageBits.push_back("Depth stencil attachment");
  }
  if(flags & vk::ImageUsageFlagBits::eTransientAttachment)
  {
    imageUsageBits.push_back("Transient attachment");
  }
  if(flags & vk::ImageUsageFlagBits::eInputAttachment)
  {
    imageUsageBits.push_back("Input attachment");
  }

  return imageUsageBits;
}

std::vector<std::string> logPresentModeBits(const std::vector<vk::PresentModeKHR>& flags)
{
  std::vector<std::string> presentModeBits;
  for(const auto& flag : flags)
  {
    if(flag == vk::PresentModeKHR::eImmediate)
    {
      presentModeBits.push_back("Immediate");
    }
    if(flag == vk::PresentModeKHR::eMailbox)
    {
      presentModeBits.push_back("Mailbox");
    }
    if(flag == vk::PresentModeKHR::eFifo)
    {
      presentModeBits.push_back("Fifo");
    }
    if(flag == vk::PresentModeKHR::eFifoRelaxed)
    {
      presentModeBits.push_back("Fifo relaxed");
    }
  }

  return presentModeBits;
}