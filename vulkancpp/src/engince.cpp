#include "engine.hpp"

Engine::Engine()
{
  buildGLFWWindow();
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
}

void Engine::buildGLFWWindow()
{
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  if(window = glfwCreateWindow(width, height, title, nullptr, nullptr))
  {
    if(debugMode)
    {
      std::cout << "GLFW window created succesfully with width: " << width << ", height: " << height << "\n";
    }
  }
  else 
  {
    throw std::runtime_error("Failed to create GLFW window\n");
  }
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
  if(physicalDevice == nullptr)
  {
    throw std::runtime_error("Failed to find a suitable GPU\n");
  }
  auto result = createLogicalDevice(physicalDevice, surface, debugMode);
  device = result.first;
  graphicsQueue = result.second.first;
  presentQueue = result.second.second;
}

Engine::~Engine()
{
  if(debugMode)
  {
    std::cout << "Engine being destroyed\n";
    instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, dispatchLoader);
  }
  device.destroy();
  instance.destroySurfaceKHR(surface);
  instance.destroy();
  glfwTerminate();
}