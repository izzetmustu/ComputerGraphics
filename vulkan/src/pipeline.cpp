#include "pipeline.hpp"

std::vector<char> Pipeline::readFile(const std::string& filename){
  std::ifstream file(filename, std::ios::ate | std::ios::binary);
  if(!file.is_open()){
    throw std::runtime_error("Failed to open file: " + filename);
  }

  size_t fileSize = static_cast<size_t>(file.tellg());
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);
  file.close();

  return buffer;
};

void Pipeline::createGraphicsPipeline(const std::string& vertFile, const std::string& fragFile){
  std::vector<char> vertCode = readFile(vertFile);
  std::vector<char> fragCode = readFile(fragFile);

  std::cout << "Vertex Shader Code Size: " << vertCode.size() << std::endl;
  std::cout << "Fragment Shader Code Size: " << fragCode.size() << std::endl;
};

Pipeline::Pipeline(const std::string& vertFile, const std::string& fragFile){
  createGraphicsPipeline(vertFile, fragFile);
};