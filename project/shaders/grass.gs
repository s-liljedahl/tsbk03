// Bigger pass-through including normal and texture coords

#version 150

layout(triangles) in;
layout(line_strip, max_vertices = 9) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

uniform float t;

out vec3 frag_Position;

const float magnitude = 0.1;

void GenerateLine(int index)
{
  // base 
  gl_Position = gl_in[index].gl_Position;
  EmitVertex();
  // base 
  gl_Position =  (gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * magnitude);
  EmitVertex();  
  EndPrimitive();
}

void main()
{
  GenerateLine(0);
  GenerateLine(1);
  GenerateLine(2);
}
