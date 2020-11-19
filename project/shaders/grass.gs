// Bigger pass-through including normal and texture coords

#version 150

uniform float t;
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 exNormal[3];
out vec3 exNormalG;

const float magnitude = 0.1;

// in vec4 gl_Position[3]
// out vec4 gl_Position 

vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
} 

vec4 middle() {
  vec4 middle = vec4(0.0, 0.0, 0.0, 0.0);
  for(int i = 0; i < gl_in.length(); i++)
  {
    middle += gl_in[i].gl_Position;
  }
  middle /= gl_in.length();
  return middle;
}

void main()
{
  for(int i = 0; i < gl_in.length(); i++)
  {
    exNormalG = exNormal[i];
    // gl_Position = gl_in[i].gl_Position;
    // EmitVertex();
    gl_Position = gl_in[i].gl_Position + magnitude * vec4(GetNormal(), 1.0);
    EmitVertex();
  }
  EndPrimitive();
}
