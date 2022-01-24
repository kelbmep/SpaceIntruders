#pragma once

#include <vertexBuffer.hpp>
#include <utilities/general_functions.hpp>
#include <glm/glm.hpp>

class ParticleBuffer : public VertexBuffer
{
public:
	struct ParticleData
	{
		glm::vec2 pos;
		glm::vec2 velocity;
		utils::color color = glm::vec4{ 1.0 };
		float phase = 0;
	};

	virtual void add_data(ParticleData) = 0;

	~ParticleBuffer() = default;
};