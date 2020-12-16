// Flat shading, same normal for all

#version 150

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 exNormal[3];
out vec3 exNormalG;

vec3 getMiddle() {
  vec4 middleOfTriangle = vec4(0.0);
  vec3 avgNormal = vec3(0.0);;
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
  for(int i = 0; i < gl_in.length(); i++)
  {
    gl_Position = gl_in[i].gl_Position;
    exNormalG = getMiddle();
    EmitVertex();
  }
  EndPrimitive();
}
