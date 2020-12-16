
#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

uniform float t;  
uniform mat4 projectionMatrix;
uniform vec3 gravity;

out vec3 frag_Position;
out vec4 color;
out vec3 exNormalG; // Phong

const float magnitude = 0.2;

void CreateVert(int index, float pOffset)
{
    vec4 normal = vec4(gs_in[index].normal, 0.0) + vec4(gravity, 0.0);
    vec4 pos = gl_in[index].gl_Position;
    vec4 right = gl_in[index].gl_Position + vec4(pOffset, 0.0, 0.0, 0.0);
    vec4 left = gl_in[index].gl_Position + vec4(-pOffset, 0.0, 0.0, 0.0);
    exNormalG = gs_in[index].normal;
    // base
    gl_Position = projectionMatrix * left;
    color = vec4(0.1, 0.3, 0.1, 1.0);
    EmitVertex();
    // base
    gl_Position = projectionMatrix * right;
    color = vec4(0.1, 0.3, 0.1, 1.0);
    EmitVertex();
    // top
    gl_Position = projectionMatrix * (pos + normal * magnitude);
    color = vec4(0.2, 0.7, 0.2, 0.5);
    EmitVertex();
    EndPrimitive();
}

void main()
{
  const float offset = 0.05;

  CreateVert(0, offset);
  CreateVert(1, offset);
  CreateVert(2, offset);

  CreateVert(0, -offset);
  CreateVert(1, -offset);
  CreateVert(2, -offset);
}




