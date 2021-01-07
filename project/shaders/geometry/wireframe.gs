// Flat shading, same normal for all

#version 150

layout(triangles) in;
layout(line_strip, max_vertices = 3) out;

in vec3 exNormal[3];
out vec3 exNormalG;
out vec4 color;

vec3 getMiddle() {
  vec4 middleOfTriangle = vec4(0.0);
  vec3 avgNormal = vec3(0.0);
  for(int i = 0; i < gl_in.length(); i++)
  {
    avgNormal += exNormal[i];
  }
  middleOfTriangle /= gl_in.length();
  avgNormal /= gl_in.length();
  return(normalize(avgNormal));

}

void main()
{
  color = vec4(0.5, 1.0, 0.5, 1.0);
  for(int i = 0; i < gl_in.length(); i++)
  {
    gl_Position = gl_in[i].gl_Position;
    exNormalG = vec3(color);
    EmitVertex();
  }
  EndPrimitive();
}
