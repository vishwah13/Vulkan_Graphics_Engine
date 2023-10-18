#pragma once

#include "engine_device.hpp"
#include <string>
#include <vector>

namespace VulkanEngine {
	struct PipelineConfigurationInfo {
		VkViewport viewport;
		VkRect2D scissor;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};
	class EnginePipeline {
	public:
		EnginePipeline(EngineDevice& device,const PipelineConfigurationInfo& configInfo,const std::string& vertFilePath, const std::string& fragFilePath);
		~EnginePipeline();

		EnginePipeline(const EnginePipeline&) = delete;
		 void operators(const EnginePipeline&) = delete;

		 static PipelineConfigurationInfo defaultPipelineConfigurationInfo(uint32_t width, uint32_t height);
	private:
		static std::vector<char> readFile(const std::string& filePath);

		void createGraphicsPipeline(const PipelineConfigurationInfo& configInfo,const std::string& vertFilePath, const std::string& fragFilePath);
		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

		EngineDevice& engineDevice;
		VkPipeline graphicsPipline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;
	};
}