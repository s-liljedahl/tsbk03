// Bigger pass-through including normal and texture coords

#version 150

uniform float t;
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 exNormal[3];
out vec3 exNormalG;

const float magnitude = 0.3;

vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
} 


void main()
{
  for(int i = 0; i < gl_in.length(); i++)
  {
    gl_Position = gl_in[i].gl_Position + t * magnitude * normalize(vec4(exNormal[i], 1.0));
    exNormalG = exNormal[i];
    EmitVertex();
  }
  EndPrimitive();
}
