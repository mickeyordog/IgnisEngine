#include <iostream>
#include <map>
#include <set>

#include "graphicsContext.h"
#include "windowContext.h"

void GraphicsContext::init() {
	getInstance().createInstance();

    uint32_t extensionPropCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionPropCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionPropCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionPropCount, extensions.data());
    std::cout << "available extensions:\n";
    for (const auto& extension : extensions) {
        std::cout << '\t' << extension.extensionName << '\n';
        // TODO: check that all required extensions are in this list
    }

    setupDebugMessenger();
	createSurface();
	selectPhysicalDevice();
	createLogicalDevice();
	createSwapChain();
}

void GraphicsContext::deinit() {
    vkDestroySwapchainKHR(getInstance().device, getInstance().swapChain, nullptr);
	vkDestroyDevice(getInstance().device, nullptr);

    if (enableValidationLayers) {
        DestroyDebugUtilsMessengerEXT(getInstance().instance, getInstance().debugMessenger, nullptr);
    }

	vkDestroySurfaceKHR(getInstance().instance, getInstance().surface, nullptr);
    vkDestroyInstance(getInstance().instance, nullptr);
}

void GraphicsContext::createInstance() {

    if (enableValidationLayers && !checkValidationLayerSupport()) {
        IGNIS_ERROR("Validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "My Game";
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    appInfo.pEngineName = "Ignis Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    std::vector<const char*> extensionNames = getRequiredExtensions();

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensionNames.size());
    createInfo.ppEnabledExtensionNames = extensionNames.data();
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = nullptr;
    createInfo.pNext = nullptr;
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    else {
        createInfo.enabledLayerCount = 0;
        createInfo.ppEnabledLayerNames = nullptr;
    }
    if (vkCreateInstance(&createInfo, nullptr, &getInstance().instance) != VK_SUCCESS) {
        IGNIS_ERROR("Failed to create Vulkan instance!");
    }
    
}

std::vector<const char*> GraphicsContext::getRequiredExtensions()
{
    uint32_t extensionCount;
    if (SDL_Vulkan_GetInstanceExtensions(WindowContext::getInstance().getWindow(), &extensionCount, nullptr) < 0) {
        IGNIS_ERROR((std::string("Failed to get required Vulkan extensions from SDL! SDL Error: ") + SDL_GetError()).c_str());
    }
    std::vector<const char*> extensionNames(extensionCount);
    if (SDL_Vulkan_GetInstanceExtensions(WindowContext::getInstance().getWindow(), &extensionCount, extensionNames.data()) < 0) {
        IGNIS_ERROR((std::string("Failed to get required Vulkan extensions from SDL! SDL Error: ") + SDL_GetError()).c_str());
    }

    if (enableValidationLayers) {
        extensionNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    
    return extensionNames;
}

bool GraphicsContext::checkValidationLayerSupport()
{
    uint32_t layerCount;
    VkResult result = vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	assert(result == VK_SUCCESS);
    std::vector<VkLayerProperties> availableLayers(layerCount);
    result = vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
	assert(result == VK_SUCCESS);

    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}

void GraphicsContext::setupDebugMessenger()
{
    if (!enableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT(getInstance().instance, &createInfo, nullptr, &getInstance().debugMessenger) != VK_SUCCESS) {
        IGNIS_ERROR("Failed to set up debug messenger!");
    }
}

void GraphicsContext::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}

void GraphicsContext::createSurface() {
	if (SDL_Vulkan_CreateSurface(WindowContext::getInstance().getWindow(), getInstance().instance, &getInstance().surface) != SDL_TRUE) {
		IGNIS_ERROR((std::string("Failed to create Vulkan surface! SDL Error: ") + SDL_GetError()).c_str());
	}
}

void GraphicsContext::selectPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(getInstance().instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        IGNIS_ERROR("Failed to find GPUs with Vulkan support!");
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(getInstance().instance, &deviceCount, devices.data());

    std::multimap<int, VkPhysicalDevice> candidates;

    for (const auto& device : devices) {
        int score = rateDeviceSuitability(device);
        candidates.insert(std::make_pair(score, device));
    }

    if (candidates.rbegin()->first > 0) {
        getInstance().physicalDevice = candidates.rbegin()->second;
    }
    else {
        IGNIS_ERROR("Failed to find a suitable GPU!");
    }
}

int GraphicsContext::rateDeviceSuitability(const VkPhysicalDevice& device)
{
    int score = 0;

    QueueFamilyIndices indices = findQueueFamilies(device);
	if (!indices.isComplete()) {
		return score;
	}

	if (!checkDeviceExtensionSupport(device)) {
		return score;
	}

    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
    if (swapChainSupport.formats.empty() || swapChainSupport.presentModes.empty()) {
		return score;
    }

    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    // Discrete GPUs have a significant performance advantage
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }

    // Maximum possible size of textures affects graphics quality
    score += deviceProperties.limits.maxImageDimension2D;

    return score;
}

bool GraphicsContext::checkDeviceExtensionSupport(const VkPhysicalDevice& device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

QueueFamilyIndices GraphicsContext::findQueueFamilies(const VkPhysicalDevice& device)
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, getInstance().surface, &presentSupport);
        if (presentSupport) {
            indices.presentFamily = i;
        }
		// TODO: could add logic to prefer a physical device that supports both graphics and present for improved performance

        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}

void GraphicsContext::createLogicalDevice() {
    QueueFamilyIndices indices = findQueueFamilies(getInstance().physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };
    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());;
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(getInstance().physicalDevice, &createInfo, nullptr, &getInstance().device) != VK_SUCCESS) {
        IGNIS_ERROR("Failed to create logical device!");
    }

	vkGetDeviceQueue(getInstance().device, indices.graphicsFamily.value(), 0, &getInstance().graphicsQueue);
	vkGetDeviceQueue(getInstance().device, indices.presentFamily.value(), 0, &getInstance().presentQueue);
}

void GraphicsContext::createSwapChain() {
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(getInstance().physicalDevice);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = getInstance().surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = findQueueFamilies(getInstance().physicalDevice);
    uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(getInstance().device, &createInfo, nullptr, &getInstance().swapChain) != VK_SUCCESS) {
        IGNIS_ERROR("Failed to create swap chain!");
    }
}

SwapChainSupportDetails GraphicsContext::querySwapChainSupport(const VkPhysicalDevice& device) {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, getInstance().surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, getInstance().surface, &formatCount, nullptr);
    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, getInstance().surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, getInstance().surface, &presentModeCount, nullptr);
    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, getInstance().surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

VkSurfaceFormatKHR GraphicsContext::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
	assert(!availableFormats.empty());
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

	std::cout << "No preferred swap surface format found, choosing first available format" << std::endl;
    return availableFormats[0];
}

VkPresentModeKHR GraphicsContext::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    //for (const auto& availablePresentMode : availablePresentModes) {
    //    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
    //        return availablePresentMode;
    //    }
    //}

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D GraphicsContext::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }
    else {
        int width, height;
		SDL_Vulkan_GetDrawableSize(WindowContext::getInstance().getWindow(), &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}
