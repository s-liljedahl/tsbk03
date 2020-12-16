// Bigger pass-through including normal and texture coords

#version 150

uniform float t;
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 exNormal[3];
out vec3 exNormalG;

// in vec4 gl_Position[3]
// out vec4 gl_Position 

vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}  

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 1.0;
    vec3 direction = normal * t * magnitude; 
    return position + vec4(direction, 0.0);
} 

void main()
{

  vec4 middle = vec4(0.0, 0.0, 0.0, 0.0);
  for(int i = 0; i < gl_in.length(); i++)
  {
    middle += gl_in[i].gl_Position;
  }
  middle /= gl_in.length();

  for(int i = 0; i < gl_in.length(); i++)
  {
    // gl_Position = explode(gl_in[i].gl_Position + (middle * t * 2), GetNormal());
    gl_Position = explode(gl_in[i].gl_Position, GetNormal());
    exNormalG = exNormal[i];
    EmitVertex();
  }
  EndPrimitive();
}
