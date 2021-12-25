#include <ParticleEmitter.hpp>
#include <engine.hpp>
#include <particleBuffer.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <random>

/*ParticleEmitter::ParticleEmitter(const Engine& engine, size_t count) : Node(engine)
{
	std::vector<ParticleBuffer::ParticleData> particles;
	particles.reserve(count);

	auto start_pos = (get_transform() * glm::vec3(1.0f)).xy();

	std::random_device rd;
	static std::mt19937 gen{};
	static std::uniform_real_distribution<float> angle_gen(0, 360);
	static std::uniform_real_distribution<float> velocity_gen(500, 800);

	for (size_t i = 0; i < count; i++)
	{
		ParticleBuffer::ParticleData particle;
		particle.pos = start_pos;

		auto vector = glm::rotate(glm::vec2(1.0f, 0.0f), glm::radians(angle_gen));
		particle.velocity = vector * speed;
	}
}*/