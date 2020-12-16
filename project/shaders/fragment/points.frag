#version 150

out vec4 outColor;
in vec3 fColor;

void main()
{
    outColor = vec4(fColor, 1.0);   
}