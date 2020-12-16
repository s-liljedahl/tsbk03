
#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

uniform float t;  
uniform mat4 projectionMatrix;
uniform vec3 gravity;

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

void GenerateLine2(int index, float pOffset, float nOffset)
{
    vec4 pos = gl_in[index].gl_Position + vec4(0.0, pOffset, 0.0, 0.0);
    vec4 normal = vec4(gs_in[index].normal, 0.0) + vec4(gravity, 0.0);
    // base
    gl_Position = projectionMatrix * pos;
    EmitVertex();
    // top
    gl_Position = projectionMatrix * (pos + normal * magnitude);
    EmitVertex();
    EndPrimitive();
}

void main()
{
  GenerateLine2(0, 0, -1.0);
  GenerateLine2(0, 0, -0.5);
  GenerateLine2(0, 0, -0.8);
  
  GenerateLine2(1, 0, -1.0);
  GenerateLine2(1, 0, -0.5);
  GenerateLine2(1, 0, -0.8);

  GenerateLine2(2, 0, -1.0);
  GenerateLine2(2, 0, -0.5);
  GenerateLine2(2, 0, -0.8);
}




