
#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

uniform float t;  
uniform mat4 projectionMatrix;

out vec3 frag_Position;
const float magnitude = 0.5;

void GenerateLine(int index)
{
    // base
    gl_Position = projectionMatrix * gl_in[index].gl_Position;
    EmitVertex();
    // top
    gl_Position = projectionMatrix * (gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * magnitude);
    EmitVertex();
    EndPrimitive();
}

void main()
{
  GenerateLine(0);
  GenerateLine(1);
  GenerateLine(2);
}




