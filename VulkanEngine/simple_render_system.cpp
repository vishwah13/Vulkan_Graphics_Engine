#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "app.hpp"
#include <stdexcept>
#include <array>
#include <assert.h>

#include "simple_render_system.hpp"

namespace VulkanEngine {

	struct SimplePushConstantData {
		glm::mat4 modelMatrix{1.f};
		glm::mat4 normalMatrix{1.0f};
	};

	SimpleRenderSystem::SimpleRenderSystem(EngineDevice& device, VkRenderPass renderpass, VkDescriptorSetLayout globalSetLayout) : engineDevice(device)
	{
		createPipelineLayout(globalSetLayout);
		createPipeline(renderpass);
	}
	SimpleRenderSystem::~SimpleRenderSystem()
	{
		vkDestroyPipelineLayout(engineDevice.device(), pipelineLayout, nullptr);
	}
	void SimpleRenderSystem::renderGameobjects(FrameInfo& frameInfo, std::vector<GameObject>& gameObjects)
	{
		enginePipline->bind(frameInfo.commandbuffer);

		vkCmdBindDescriptorSets(
			frameInfo.commandbuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			pipelineLayout,
			0,
			1,
			&frameInfo.globalDescriptorSet,
			0,
			nullptr);
		
		for (auto& obj : gameObjects)
		{

			SimplePushConstantData push{};
			
			push.modelMatrix = obj.transform.mat4();
			push.normalMatrix = obj.transform.normalMatrix();

			
			vkCmdPushConstants(frameInfo.commandbuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
			obj.model->bind(frameInfo.commandbuffer);
			obj.model->draw(frameInfo.commandbuffer);
		}
	}
	void SimpleRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);

		std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout};

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

		if (vkCreatePipelineLayout(engineDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("FAILED TO CREATE PIPLINE LAYOUT !!!");
		}
	}
	void SimpleRenderSystem::createPipeline(VkRenderPass renderPass)
	{
		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

		PipelineConfigurationInfo pipelineConfig{};
		EnginePipeline::defaultPipelineConfigurationInfo(pipelineConfig);
		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = pipelineLayout;
		enginePipline = std::make_unique<EnginePipeline>(engineDevice, pipelineConfig, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv");
	}
}

