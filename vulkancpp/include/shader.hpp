#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <vulkan/vulkan.hpp>

std::vector<char> readFile(const std::string& filename, bool debug);
vk::ShaderModule createShaderModule(std::string filename, vk::Device device, const bool& debug);