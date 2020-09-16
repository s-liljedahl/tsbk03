#version 150
// bump mapping should be calculated
// 1) in view coordinates
// 2) in texture coordinates

in vec2 outTexCoord;
in vec3 out_Normal;
in vec3 Ps;
in vec3 Pt;
in vec3 pixPos;  // Needed for specular reflections
uniform sampler2D texUnit, tex;
out vec4 out_Color;

void main(void)
{

  vec3 light = vec3(0.0, 0.7, 0.7); // Light source in view coordinates
	
	// Calculate gradients here
	float offset = 1.0 / 256.0; // texture size, same in both directions

	float ds = texture(texUnit, outTexCoord + vec2(offset, 0.0)).g; 
	float dt = texture(texUnit, outTexCoord + vec2(0.0, offset)).g; 
	float db = texture(texUnit, outTexCoord).g;

	float bs = (db - ds) * 8;
	float bt = (db - dt) * 8;

  vec3 normal = normalize(out_Normal);
	
	vec3 new_normal = normalize(normal + bs * Ps + bt * Pt);

	vec4 tex2 = texture(tex, outTexCoord) * vec4(1.0, 0.0, 0.0, 0.0);

	// Simplified lighting calculation.
	// A full solution would include material, ambient, specular, light sources, multiply by texture.
	out_Color = vec4( dot(new_normal, light)) + tex2;
	// out_Color =  vec4(dot(tex2, light));

}
