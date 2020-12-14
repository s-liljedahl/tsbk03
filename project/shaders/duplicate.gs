// Bigger pass-through including normal and texture coords

#version 150

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 exNormal[3];
in vec3 exSurface[3];

out vec3 exNormalG;
out vec3 frag_Position;
out vec3 exSurfaceG;

void main()
{

  for(int i = 0; i < gl_in.length(); i++)
  {
    gl_Position = gl_in[i].gl_Position + vec4(3.0, 0.0, 0.0, 0.0);
    exNormalG = exNormal[i];
    exSurfaceG = exSurface[i];
    frag_Position = vec3(gl_Position);
    
    EmitVertex();
  }

  EndPrimitive();
}
