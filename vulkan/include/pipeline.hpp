#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <iostream>

class Pipeline{
  public:
    Pipeline(const std::string& vertFile, const std::string& fragFile);
    Pipeline(const Pipeline&) = delete;
    Pipeline& operator=(const Pipeline&) = delete;
    ~Pipeline(){};
  private:
    static std::vector<char> readFile(const std::string& filename);
    void createGraphicsPipeline(const std::string& vertFile, const std::string& fragFile);
};