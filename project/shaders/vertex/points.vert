#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;

uniform mat4 projectionMatrix;
uniform mat4 modelToWorldToView;
uniform mat4 viewMatrix;

out VS_OUT {
    vec3 color;
} vs_out;


void main(void)
{
    // Rotate normal (simple normal matrix, uniform scaling only)
    gl_Position = modelToWorldToView * vec4(inPosition, 1.0); 
    mat3 normalMatrix = mat3(transpose(inverse(modelToWorldToView)));
    vs_out.color = normalize(vec3(vec4(normalMatrix * inNormal, 0.0)));
}
