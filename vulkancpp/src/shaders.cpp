#include "shader.hpp"

std::vector<char> readFile(const std::string& filename, bool debug)
{
  std::ifstream file(filename, std::ios::ate | std::ios::binary);
  if(!file.is_open())
  {
    throw std::runtime_error("Failed to open file" + filename + "\n");
  }

  size_t fileSize = static_cast<size_t>(file.tellg());
  std::vector<char> buffer(fileSize);
  file.seekg(0);
  file.read(buffer.data(), fileSize);
  file.close();

  if(debug)
  {
    std::cout << "File read: " << filename << "\n";
  }

  return buffer;
}

vk::ShaderModule createShaderModule(std::string filename, vk::Device device, const bool& debug)
{
  std::vector<char> code = readFile(filename, debug);
  vk::ShaderModuleCreateInfo moduleInfo = {};
  moduleInfo.flags = vk::ShaderModuleCreateFlags();
  moduleInfo.codeSize = code.size();
  moduleInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

  try
  {
    return device.createShaderModule(moduleInfo);
  }
  catch(vk::SystemError& e)
  {
    std::cerr << e.what() << '\n';
    throw std::runtime_error("Failed to create shader module" + filename + "\n");
  }
}