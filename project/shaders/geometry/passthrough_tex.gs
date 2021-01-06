// Bigger pass-through including normal and texture coords

#version 150

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 texCoord[3]; 
in vec3 exNormal[3];
in vec3 exSurface[3];

uniform sampler2D tex;

out vec3 exNormalG;
out vec3 frag_Position;
out vec3 exSurfaceG;
out vec2 texCoordG;

void main()
{

  for(int i = 0; i < gl_in.length(); i++)
  {
    exNormalG = exNormal[i];
    texCoordG = texCoord[i];
    vec4 texColor = texture(tex, texCoordG);

    if (texColor.g <= 0.6) {
      gl_Position = gl_in[i].gl_Position + (vec4(exNormalG, 1.0) * 0.07);
    } else {
      gl_Position = gl_in[i].gl_Position;
    }
    exSurfaceG = exSurface[i];
    frag_Position = vec3(gl_Position);
    EmitVertex();
  }
  EndPrimitive();
}
