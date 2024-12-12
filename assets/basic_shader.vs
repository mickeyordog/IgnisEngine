#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(aPos, 1.0);
    // gl_Position = vec4(vec2(aTexCoords.x, 1.0 - aTexCoords.y) * 2.0 - 1.0, 0.0, 1.0);
    TexCoords = aTexCoords;
}