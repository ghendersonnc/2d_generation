#version 330

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

out vec4 colorMultiplier;

uniform mat4 model;
uniform mat4 projection;

void main() {
    gl_Position = projection * model * vec4(inPosition.xy, 1.0f, 1.0f);
    colorMultiplier = vec4(inColor, 1.0f);
}