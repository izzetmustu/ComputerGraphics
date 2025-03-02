#pragma once

#include <vulkan/vulkan.hpp>
#include <iostream>

void logDeviceProperties(const vk::PhysicalDevice& device);

/*
	* Debug call back:
	*
	*	typedef enum VkDebugUtilsMessageSeverityFlagBitsEXT {
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT = 0x00000001,
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT = 0x00000010,
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT = 0x00000100,
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT = 0x00001000,
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT = 0x7FFFFFFF
		} VkDebugUtilsMessageSeverityFlagBitsEXT;

	*	typedef enum VkDebugUtilsMessageTypeFlagBitsEXT {
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT = 0x00000001,
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT = 0x00000002,
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT = 0x00000004,
			VK_DEBUG_UTILS_MESSAGE_TYPE_FLAG_BITS_MAX_ENUM_EXT = 0x7FFFFFFF
		} VkDebugUtilsMessageTypeFlagBitsEXT;

	*	typedef struct VkDebugUtilsMessengerCallbackDataEXT {
			VkStructureType                              sType;
			const void*                                  pNext;
			VkDebugUtilsMessengerCallbackDataFlagsEXT    flags;
			const char*                                  pMessageIdName;
			int32_t                                      messageIdNumber;
			const char*                                  pMessage;
			uint32_t                                     queueLabelCount;
			const VkDebugUtilsLabelEXT*                  pQueueLabels;
			uint32_t                                     cmdBufLabelCount;
			const VkDebugUtilsLabelEXT*                  pCmdBufLabels;
			uint32_t                                     objectCount;
			const VkDebugUtilsObjectNameInfoEXT*         pObjects;
		} VkDebugUtilsMessengerCallbackDataEXT;

*/

/**
    Logging callback function.

    \param messageSeverity describes the severity level of the message
    \param messageType describes the type of the message
    \param pCallbackData standard data associated with the message
    \param pUserData custom extra data which can be associated with the message
    \returns whether to end program execution
*/
VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData
);

/**
    Make a debug messenger

    \param instance The Vulkan instance which will be debugged.
    \param dldi dynamically loads instance based dispatch functions
    \returns the created messenger
*/
vk::DebugUtilsMessengerEXT makeDebugMessenger(vk::Instance& instance, vk::DispatchLoaderDynamic& dldi);

std::vector<std::string> logTransformBits(const vk::SurfaceTransformFlagsKHR& flags);
std::vector<std::string> logCompositeAlphaBits(const vk::CompositeAlphaFlagsKHR& flags);
std::vector<std::string> logImageUsageBits(const vk::ImageUsageFlags& flags);
std::vector<std::string> logPresentModeBits(const std::vector<vk::PresentModeKHR>& flags);