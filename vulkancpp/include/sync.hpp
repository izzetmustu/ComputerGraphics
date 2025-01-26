#pragma once

#include <vulkan/vulkan.hpp>
#include <vector>
#include <iostream>
#include <stdexcept>

#include "frame.hpp"

vk::Semaphore createSemaphore(const vk::Device& device, const bool& debug);
vk::Fence createFence(const vk::Device& device, const bool& debug);
