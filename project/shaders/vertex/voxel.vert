#version 330 core

in vec3 inPosition;
in vec3 inNormal;

uniform mat4 projectionMatrix;
uniform mat4 modelToWorldToView;
uniform mat4 viewMatrix;

void main(void)
{
    // Rotate normal (simple normal matrix, uniform scaling only)
    gl_Position = modelToWorldToView * vec4(inPosition, 1.0); 
}
