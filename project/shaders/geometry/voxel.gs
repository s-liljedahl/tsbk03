// Bigger pass-through including normal and texture coords

#version 150

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 exNormal[3];
in vec3 exSurface[3];

out vec3 exNormalG;
out vec3 frag_Position;
out vec3 exSurfaceG;

void AddQuad(vec4 center, vec4 dy, vec4 dx) {
    fColor = gColor[0];
    gl_Position = center + (dx - dy);
    EmitVertex();
    fColor = gColor[0];
    gl_Position = center + (-dx - dy);
    EmitVertex();
    fColor = gColor[0];
    gl_Position = center + (dx + dy);
    EmitVertex();
    fColor = gColor[0];
    gl_Position = center + (-dx + dy);
    EmitVertex();

    EndPrimitive();
}

void main()
{
  vec4 center = gl_in[0].gl_Position;
    
  vec4 dx = mvp[0] / 2.0f * voxSize;
  vec4 dy = mvp[1] / 2.0f * voxSize;
  vec4 dz = mvp[2] / 2.0f * voxSize;


  for(int i = 0; i < gl_in.length(); i++)
  {
    gl_Position = gl_in[i].gl_Position;
    exNormalG = exNormal[i];
    exSurfaceG = exSurface[i];
    frag_Position = vec3(gl_Position);
    
    EmitVertex();
  }

  EndPrimitive();
}
