
#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

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

void GenerateLine(int index, float pOffset, float nOffset)
{
    vec4 normal = vec4(gs_in[index].normal, 0.0) + vec4(gravity, 0.0);
    vec4 pos = gl_in[index].gl_Position;
    vec4 right = gl_in[index].gl_Position + vec4(pOffset, 0.0, 0.0, 0.0);
    vec4 left = gl_in[index].gl_Position + vec4(-pOffset, 0.0, 0.0, 0.0);
    exNormalG = gs_in[index].normal;
    // base
    gl_Position = projectionMatrix * left;
    color = vec4(1.0, 0.0, 0.2, 1.0);
    EmitVertex();
    // base
    gl_Position = projectionMatrix * right;
    color = vec4(1.0, 0.2, 0.2, 1.0);
    EmitVertex();
    // top
    gl_Position = projectionMatrix * (pos + normal * magnitude);
    color = vec4(1.0, 1.0, 1.0, 0.0);
    EmitVertex();
    EndPrimitive();
}

void main()
{
  const float offset = 0.01;
  GenerateLine(0, offset, -1.0);
  GenerateLine(1, offset, -0.5);
  GenerateLine(2, offset, -0.8);

  // GenerateLine(1, offset, -1.0);
  // GenerateLine(1, offset, -0.5);
  // GenerateLine(1, offset, -0.8);

  // GenerateLine(2, offset, -1.0);
  // GenerateLine(2, offset, -0.5);
  // GenerateLine(2, offset, -0.8);
}




