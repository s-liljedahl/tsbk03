// Bigger pass-through including normal and texture coords

#version 150

uniform float t;
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 exNormal[3];
out vec3 exNormalG;

// in vec4 gl_Position[3]
// out vec4 gl_Position 

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
    gl_Position = gl_in[i].gl_Position +
                  (middle * 0.5 * (t + 1)) +
                  ((t + 1) * 0.5 * normalize(vec4(exNormal[i], 1.0)));
    exNormalG = exNormal[i];
    EmitVertex();
  }
  EndPrimitive();
}
