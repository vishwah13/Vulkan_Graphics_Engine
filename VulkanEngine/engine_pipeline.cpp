#include "engine_pipeline.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace VulkanEngine {

	EnginePipeline::EnginePipeline(EngineDevice& device, const PipelineConfigurationInfo& configInfo, const std::string& vertFilePath, const std::string& fragFilePath) : engineDevice(device)
	{
		createGraphicsPipeline(configInfo, vertFilePath, fragFilePath);
	}

	PipelineConfigurationInfo EnginePipeline::defaultPipelineConfigurationInfo(uint32_t width, uint32_t height)
	{
		return PipelineConfigurationInfo();
	}

	std::vector<char> EnginePipeline::readFile(const std::string& filePath)
	{
		std::ifstream file(filePath, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			throw std::runtime_error("FAILED TO OPEN FILE " + filePath);
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}

	void EnginePipeline::createGraphicsPipeline(const PipelineConfigurationInfo& configInfo, const std::string& vertFilePath, const std::string& fragFilePath)
	{
		auto vertCode = readFile(vertFilePath);
		auto fragCode = readFile(fragFilePath);

		std::cout << vertCode.size() << '\n';
		std::cout << fragCode.size() << '\n';
	}

	void EnginePipeline::createShaderModule(const std::vector<char>& code, VkShaderModule shaderModule)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		if (vkCreateShaderModule(engineDevice.device(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
			throw std::runtime_error("failed to create shader module!");
		}
	}

}

