#pragma once

#include <render.hpp>
//#include <GL/GLParticleBuffer.hpp>
#include <SDL.h>
#include <unordered_map>

class Engine;
class Framebuffer;
class TextureUniform;
class Vec2Uniform;
class Vec3Uniform;
class FloatUniform;
class GLVertexBuffer;
class GLParticleBuffer;

class GLRender final : public Render
{
public:
	explicit GLRender(const Engine& engine, SDL_Window* window);

	void init() override;

	void draw() override;
	
	std::shared_ptr<VertexBuffer> create_vertex_buffer(MeshData) const override;
	std::shared_ptr<VertexBuffer> create_vertex_buffer(const VertexData&) const override;
	
	std::shared_ptr<ShaderProgram> create_program(std::string) const override;
	
	std::shared_ptr<ParticleBuffer> create_particle_buffer(std::vector<ParticleBuffer::ParticleData>) const override;
	std::shared_ptr<ParticleBuffer> create_particle_buffer(size_t) const override;
	
	std::shared_ptr<Texture> create_texture(Bitmap, bool) const override;

	glm::vec2 get_render_resolution() const override; 
private:
	const Engine& _engine;
	std::unique_ptr<void, void(*)(void*)> _draw_context;

	mutable std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> _programs;

	std::shared_ptr<ShaderProgram> _pp_program;
	std::shared_ptr<ShaderProgram> _blur_program;

	std::shared_ptr<Framebuffer> _orig_fbo;
	std::shared_ptr<Framebuffer> _bloom_fbo;
	std::shared_ptr<Framebuffer> _blur_fbo;

	std::shared_ptr<TextureUniform>  _texture_uniform;
	std::shared_ptr<TextureUniform>  _texture2_uniform;
	std::shared_ptr<Vec3Uniform>  _distort_uniform;
	std::shared_ptr<FloatUniform>  _time_uniform;

	std::shared_ptr<GLVertexBuffer>  _buffer;

	std::shared_ptr<TextureUniform>  _blur_texture_uniform;
	std::shared_ptr<Vec2Uniform>  _blur_rad_uniform;
};
