#pragma once

#include <glm/glm.hpp>
#include <vertexBuffer.hpp>

class ParticleBuffer : public VertexBuffer
{
public:
	struct ParticleData
	{
		glm::vec2 pos;
		glm::vec2 velocity;
		color color;
	};

	~ParticleBuffer() = default;
};