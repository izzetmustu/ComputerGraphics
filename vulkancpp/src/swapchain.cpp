#include "swapchain.hpp"

SwapChainSupportDetails querySwapChainSupport(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface, const bool& debug)
{
  SwapChainSupportDetails details;
  
  /*
  SurfaceCapabilitiesKHR {
    uint32_t minImageCount;
    uint32_t maxImageCount;
    Extent2D currentExtent;
    Extent2D minImageExtent;
    Extent2D maxImageExtent;
    uint32_t maxImageArrayLayers;
    SurfaceTransformFlagsKHR supportedTransforms;
    SurfaceTransformFlagBitsKHR currentTransform;
    CompositeAlphaFlagsKHR supportedCompositeAlpha;
    ImageUsageFlags supportedUsageFlags;
  };
  */
  details.capabilities = device.getSurfaceCapabilitiesKHR(surface);
  if(debug)
  {
    std::cout << "Swap chain capabilities queried:\n";
    std::cout << "\tMin image count: " << details.capabilities.minImageCount << "\n";
    std::cout << "\tMax image count: " << details.capabilities.maxImageCount << "\n";
    std::cout << "\tCurrent extent: " << details.capabilities.currentExtent.width << "x" << details.capabilities.currentExtent.height << "\n";
    std::cout << "\tMin image extent: " << details.capabilities.minImageExtent.width << "x" << details.capabilities.minImageExtent.height << "\n";
    std::cout << "\tMax image extent: " << details.capabilities.maxImageExtent.width << "x" << details.capabilities.maxImageExtent.height << "\n";
    std::cout << "\tMax image array layers: " << details.capabilities.maxImageArrayLayers << "\n";
    
    std::cout << "\tSupported transforms:\n";
    std::vector<std::string> transformBits = logTransformBits(details.capabilities.supportedTransforms);
    for(const auto& transform : transformBits)
    {
      std::cout << "\t\t" << transform << "\n";
    }
    
    std::cout << "\tCurrent transforms:\n";
    std::vector<std::string> currentTransformBits = logTransformBits(details.capabilities.currentTransform);
    for(const auto& transform : currentTransformBits)
    {
      std::cout << "\t\t" << transform << "\n";
    }
    
    std::cout << "\tSupported composite alpha:\n";
    std::vector<std::string> compositeAlphaBits = logCompositeAlphaBits(details.capabilities.supportedCompositeAlpha);
    for(const auto& alpha : compositeAlphaBits)
    {
      std::cout << "\t\t" << alpha << "\n";
    }

    std::cout << "\tSupported usage flags:\n";
    std::vector<std::string> usageBits = logImageUsageBits(details.capabilities.supportedUsageFlags);
    for(const auto& usage : usageBits)
    {
      std::cout << "\t\t" << usage << "\n";
    }
  }

  /*
  SurfaceFormatKHR {
    Format format;
    ColorSpaceKHR colorSpace;
  };
  */
  details.formats = device.getSurfaceFormatsKHR(surface);
  if(debug)
  {
    std::cout << details.formats.size() << " surface formats supported\n";
    for(const auto& format : details.formats)
    {
      std::cout << "\tFormat: " << vk::to_string(format.format) << "\n";
      std::cout << "\tColor space: " << vk::to_string(format.colorSpace) << "\n";
    }
  }

  /*
  PresentModeKHR {
    Immediate = 0,
    Mailbox = 1,
    Fifo = 2,
    FifoRelaxed = 3
  };
  */
  details.presentModes = device.getSurfacePresentModesKHR(surface);
  if(debug)
  { 
    std::cout << details.presentModes.size() << " present modes supported\n";
    std::vector<std::string> presentModeBits = logPresentModeBits(details.presentModes);
    for(const auto& mode : presentModeBits)
    {
      std::cout << "\t" << mode << "\n";
    }

    std::cout << "Swap chain support queried\n";
  }

  return details;
}

vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats)
{
  if(availableFormats.size() == 1 && availableFormats[0].format == vk::Format::eUndefined)
  {
    return {vk::Format::eB8G8R8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear};
  }

  for(const auto& format : availableFormats)
  {
    if(format.format == vk::Format::eB8G8R8A8Unorm && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
    {
      return format;
    }
  }

  return availableFormats[0];
}

vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes)
{
  vk::PresentModeKHR bestMode = vk::PresentModeKHR::eFifo;
  for(auto mode : availablePresentModes)
  {
    if(mode == vk::PresentModeKHR::eMailbox)
    {
      return mode;
    }
    else if(mode == vk::PresentModeKHR::eImmediate)
    {
      bestMode = mode;
    }
  }

  return bestMode;
}

vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, int width, int height)
{
  if(capabilities.currentExtent.width != UINT32_MAX)
  {
    return capabilities.currentExtent;
  }
  else
  {
    vk::Extent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
    actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
    actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
    return actualExtent;
  }
}

SwapChain crateSwapChain(const vk::PhysicalDevice& physicalDevice, const vk::Device& device, const vk::SurfaceKHR& surface, const int& width, const int& height, const bool& debug)
{
  SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, surface, debug);
  vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
  vk::PresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
  vk::Extent2D extent = chooseSwapExtent(swapChainSupport.capabilities, width, height);

  uint32_t imageCount = std::min(swapChainSupport.capabilities.minImageCount + 1, swapChainSupport.capabilities.maxImageCount);
  
  /*
    SwapchainCreateInfoKHR {
        SwapchainCreateFlagsKHR flags;
        SurfaceKHR surface;
        uint32_t minImageCount;
        Format imageFormat;
        ColorSpaceKHR imageColorSpace;
        Extent2D imageExtent;
        uint32_t imageArrayLayers;
        ImageUsageFlags imageUsage;
        SharingMode imageSharingMode;
        uint32_t queueFamilyIndexCount;
        const uint32_t* pQueueFamilyIndices;
        SurfaceTransformFlagBitsKHR preTransform;
        CompositeAlphaFlagBitsKHR compositeAlpha;
        PresentModeKHR presentMode;
        bool clipped;
        SwapchainKHR oldSwapchain;
    };
  */
  vk::SwapchainCreateInfoKHR createInfo = vk::SwapchainCreateInfoKHR(
    vk::SwapchainCreateFlagsKHR(),
    surface,
    imageCount,
    surfaceFormat.format,
    surfaceFormat.colorSpace,
    extent,
    1,
    vk::ImageUsageFlagBits::eColorAttachment
  );

  QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface, debug);
  uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};
  if(indices.graphicsFamily.value() != indices.presentFamily.value())
  {
    if(debug)
    {
      std::cout << "Queue families are different\n";
    }
    createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices;
  }
  else
  {
    createInfo.imageSharingMode = vk::SharingMode::eExclusive;
  }

  createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
  createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;
  createInfo.oldSwapchain = vk::SwapchainKHR(VK_NULL_HANDLE);

  SwapChain swapChain;
  try
  {
    swapChain.swapChain = device.createSwapchainKHR(createInfo);
  }
  catch(vk::SystemError& e)
  {
    std::cerr << e.what() << '\n';
    throw std::runtime_error("Failed to create swap chain\n");
  }
  
  std::vector<vk::Image> images = device.getSwapchainImagesKHR(swapChain.swapChain);
  swapChain.frames.resize(images.size());
  for(size_t i = 0; i < images.size(); i++)
  {
    /*
    ImageViewCreateInfo {
      ImageViewCreateFlags flags;
      Image image;
      ImageViewType viewType;
      Format format;
      ComponentMapping components;
      ImageSubresourceRange subresourceRange;
    };
    */
    vk::ImageViewCreateInfo viewInfo = {};
    viewInfo.image = images[i];
    viewInfo.viewType = vk::ImageViewType::e2D;
    viewInfo.components.r = vk::ComponentSwizzle::eIdentity;
    viewInfo.components.g = vk::ComponentSwizzle::eIdentity;
    viewInfo.components.b = vk::ComponentSwizzle::eIdentity;
    viewInfo.components.a = vk::ComponentSwizzle::eIdentity;
    viewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;
    viewInfo.format = surfaceFormat.format;
    
    swapChain.frames[i].image = images[i];
    swapChain.frames[i].imageView = device.createImageView(viewInfo);
  }
  swapChain.format = surfaceFormat.format;
  swapChain.extent = extent;

  return swapChain;
}