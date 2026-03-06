#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aCol;

out vec4 vertexColor;

uniform mat4 layerMatrix;

void main() {
    gl_Position = layerMatrix * vec4(aPos, 0.0, 1.0);
    vertexColor = aCol;
}