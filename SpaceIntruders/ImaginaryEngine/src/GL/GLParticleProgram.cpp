#include <GL/GLParticleProgram.hpp>

namespace
{
    constexpr auto vs_program =
        R"(
#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 velocity;
layout (location = 2) in vec4 color;
layout (location = 3) in float phase;

out vec4 oColor;
out vec2 oPos;
out float oPhase;
out float oSize;


uniform vec2 uResolutionSize;
uniform vec2 uScreenSize;
uniform float uTime;
uniform mat3 uCameraMatrix;

void main()
{
    oColor = color;
    oPhase = phase;

    vec2 v = velocity * clamp(mix(1.0, 0.25, (uTime - oPhase) / 3.0), 0.0, 1.0);
    vec2 pos = position + v * (uTime - phase);
    pos = (uCameraMatrix * vec3(pos, 1.0)).xy;
    oPos = pos;
    oPos.y = uScreenSize.y - oPos.y;

    vec2 scaledPos = pos / uScreenSize;
    scaledPos.y = 1.0 - scaledPos.y;
    scaledPos = scaledPos * 2.0 - vec2(1.0);

    oSize = clamp(mix(2.0, 64.0, (uTime - oPhase) / 1.0), 2.0, 64.0) * uCameraMatrix[0][0];
    gl_PointSize = oSize / uScreenSize.y * uResolutionSize.y;
    gl_Position = vec4(scaledPos.x, scaledPos.y, 1.0, 1.0);
}
)";
    constexpr auto ps_program =
        R"(
#version 330 core

in vec4 oColor;
in vec2 oPos;
in float oPhase;
in float oSize;

out vec4 color;

uniform float uTime;
uniform vec2 uScreenSize;
uniform vec2 uResolutionSize;

void main()
{
    if ((uTime - oPhase) > 2.0)
        discard;

    float dist = distance(oPos, gl_FragCoord.xy / uResolutionSize * uScreenSize);
    if (dist > oSize / 2.0)
        discard;

    color = oColor;
    color *= clamp(mix(1.0, 0.0, (uTime - oPhase) / 2.0), 0.0, 1.0);
    color *= clamp(mix(1.0, 0.2, (uTime - oPhase) / 0.7), 0.2, 1.0);
    color.g *= clamp( mix(0.0, 1.0, (uTime - oPhase) / 0.3), 0.0, 1.0);
    color.b *= clamp(mix(0.0, 1.0, (uTime - oPhase) / 0.5), 0.0, 1.0);
    color *= clamp(mix(1.0, 0.0, dist / (oSize * 0.6)), 0.0, 1.0);
}
)";
}

GLParticleProgram::GLParticleProgram()
    : GLProgram({"position", "velocity", "color", "phase"}, vs_program, ps_program)
{
}