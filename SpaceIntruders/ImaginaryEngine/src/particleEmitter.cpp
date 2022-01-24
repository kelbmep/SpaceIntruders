#include <particleEmitter.hpp>
#include <particleBuffer.hpp>
#include <engine.hpp>
#include <shaderProgram.hpp>
#include <window.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <random>

ParticleEmitter::ParticleEmitter(const Engine &engine)
        : Node(engine)
{
    _command.program = engine.get_render().create_program("particles");

    _screen_size_uniform = _command.program->create_vec2_uniform("uScreenSize");
    _resolution_uniform = _command.program->create_vec2_uniform("uResolutionSize");
    _time_uniform = _command.program->create_float_uniform("uTime");
    _camera_uniform = _command.program->create_mat3_uniform("uCameraMatrix");

    _command.uniforms.push_back(_screen_size_uniform);
    _command.uniforms.push_back(_time_uniform);
    _command.uniforms.push_back(_camera_uniform);
    _command.uniforms.push_back(_resolution_uniform);

    _resolution_uniform->on_activate = [=]()
    {
        _resolution_uniform->value = _engine.get_render().get_render_resolution();
    };

    //this->schedule_update();
}

void ParticleEmitter::update(fseconds dt)
{
    _time_uniform->value += dt.count();

    if (_dynamic_mode)
    {
        _last_gen_time += dt.count();
        _total_time += dt.count();

        if (_last_gen_time >= _freq && _total_time < _life_time)
        {
            auto gen_count = std::min(_count, _last_gen_time / _freq);
            _last_gen_time = 0;
            auto start_pos = (get_transform() * glm::vec3(1.0)).xy();

            for (size_t i = 0; i < gen_count; ++i)
            {
                static std::mt19937 gen{};
                static std::normal_distribution<float> angle_gen(0, 360);
                static std::normal_distribution<float> vel_gen(1, 500);

                ParticleBuffer::ParticleData particle;
                particle.pos = start_pos;

                auto vector = glm::rotate(glm::vec2{1.0f, 0.0f}, glm::radians(angle_gen(gen)));
                auto speed = vel_gen(gen);

                particle.velocity = vector * speed;
                particle.color = glm::vec4(1.0, 1.0, 1.0, 1.0);
                particle.phase = _time_uniform->value;

                auto particle_buffer = std::static_pointer_cast<ParticleBuffer>(_command.vertex_buffer);
                particle_buffer->add_data(particle);
            }
        }
    }
}

void ParticleEmitter::visitSelf()
{
    const auto& win = _engine.get_window();
    _screen_size_uniform->value = _engine.get_virtual_resolution();
    _camera_uniform->value = glm::inverse(_engine.get_camera()->get_transform());

    _command.mask = _render_mask;

    _engine.get_render().add_command(_command);
}

void ParticleEmitter::start(size_t count)
{
    _time_uniform->value = 0;

    std::vector<ParticleBuffer::ParticleData> particles;
    particles.reserve(count);

    auto start_pos = (get_transform() * glm::vec3(1.0)).xy();

    static std::mt19937 gen{};
    static std::normal_distribution<float> angle_gen(0, 360);
    static std::normal_distribution<float> vel_gen(1, 500);

    for (size_t i = 0; i < count; ++i)
    {
        ParticleBuffer::ParticleData particle;
        particle.pos = start_pos;

        auto vector = glm::rotate(glm::vec2{1.0f, 0.0f}, glm::radians(angle_gen(gen)));
        auto speed = vel_gen(gen);

        particle.velocity = vector * speed;
        particle.color = glm::vec4(1.0, 1.0, 0.0, 1.0);

        particles.push_back(particle);
    }

    _command.vertex_buffer = _engine.get_render().create_particle_buffer(std::move(particles));
}

void ParticleEmitter::start(size_t count, float freq, float life_time)
{
    _count = count;
    _command.vertex_buffer = _engine.get_render().create_particle_buffer(count);
    _dynamic_mode = true;
    _freq = freq;
    _last_gen_time = 0;
    _total_time = 0;
    _life_time = life_time;
    _time_uniform->value = 0;
}