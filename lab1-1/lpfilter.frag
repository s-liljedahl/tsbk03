#version 150

uniform sampler2D texUnit;
in vec2 texCoord;
out vec4 fragColor;

// [ 1, 2, 1 ] filter, bättre att ha större

void main(void)
{
  float texSize = 512;
  float offset = 1.0 / texSize;

  vec4 c = texture(texUnit, texCoord);

  vec4 l = texture(texUnit, texCoord + vec2(offset, 0.0));
  vec4 r = texture(texUnit, texCoord + vec2(-offset, 0.0));

  vec4 u = texture(texUnit, texCoord + vec2(0.0, offset));
  vec4 d = texture(texUnit, texCoord + vec2(0.0, -offset));
  
  vec4 ul = texture(texUnit, texCoord + vec2(-offset, offset));
  vec4 dl = texture(texUnit, texCoord + vec2(-offset, -offset));
  vec4 ur = texture(texUnit, texCoord + vec2(offset, offset));
  vec4 dr = texture(texUnit, texCoord + vec2(offset, -offset));
  
  fragColor = (c + c + l + r + u + d + ul + dl + dr + ur) / 10;

}