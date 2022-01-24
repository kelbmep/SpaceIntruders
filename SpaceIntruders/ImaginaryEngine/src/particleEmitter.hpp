#pragma once

#include <node.hpp>
#include <render.hpp>
#include <chrono>

class TextureUniform;
class Vec2Uniform;
class FloatUniform;
class Mat3Uniform;

using fseconds = std::chrono::duration<float>;

class ParticleEmitter : public Node
{
public:
    ParticleEmitter(const Engine&);

    void start(size_t);
    void start(size_t, float, float);

    void update(fseconds);// override;

protected:
    void visitSelf() override;

private:
    Render::Command _command;

    std::shared_ptr<Vec2Uniform> _screen_size_uniform;
    std::shared_ptr<Vec2Uniform> _resolution_uniform;
    std::shared_ptr<FloatUniform>  _time_uniform;
    std::shared_ptr<Mat3Uniform>  _camera_uniform;

    bool _dynamic_mode = false;
    float _freq = 0;
    float _life_time = 0;
    float _last_gen_time = 0;
    float _total_time = 0;
    float _count = 0;
};