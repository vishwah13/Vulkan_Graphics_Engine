#pragma once

#include "engine_device.hpp"
#include <string>
#include <vector>

namespace VulkanEngine {
	struct PipelineConfigurationInfo {
		
		PipelineConfigurationInfo(const PipelineConfigurationInfo&) = delete;
		PipelineConfigurationInfo& operator=(const PipelineConfigurationInfo&) = delete;

		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		std::vector<VkDynamicState> dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};
	class EnginePipeline {
	public:
		EnginePipeline(EngineDevice& device,const PipelineConfigurationInfo& configInfo,const std::string& vertFilePath, const std::string& fragFilePath);
		~EnginePipeline();

		EnginePipeline(const EnginePipeline&) = delete;
		EnginePipeline& operators(const EnginePipeline&) = delete;

		 void bind(VkCommandBuffer commandBuffer);
		 static void defaultPipelineConfigurationInfo(PipelineConfigurationInfo& configInfo);
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