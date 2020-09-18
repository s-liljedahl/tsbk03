#version 150

//in vec3 in_Color;
in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;

uniform mat4 matrix;

uniform mat4 benPos0;
uniform mat4 benPos1;

uniform mat4 benRot0;
uniform mat4 benRot1;

out vec4 g_color;
const vec3 lightDir = normalize(vec3(0.3, 0.5, 1.0));

// Uppgift 3: Soft-skinning p� GPU
//
// Flytta �ver din implementation av soft skinning fr�n CPU-sidan
// till vertexshadern. Mer info finns p� hemsidan.

void main(void)
{
	mat4 benPosInv0 = inverse(benPos0);
	mat4 benPosInv1 = inverse(benPos1);

	mat4 Mben0 = benPos0 * benRot0 * benPosInv0;
	mat4 Mben1 = benPos1 * benRot1 * benPosInv1;
	
	// Skinning
	gl_Position = matrix * (in_TexCoord.x * Mben0 + in_TexCoord.y * Mben1) * vec4(in_Position, 1.0);

	// s�tt r�d+gr�n f�rgkanal till vertex Weights
	vec4 color = vec4(in_TexCoord.x, in_TexCoord.y, in_TexCoord.x, 1.0);

	// L�gg p� en enkel ljuss�ttning p� vertexarna 	
	float intensity = dot(in_Normal, lightDir);
	color.xyz *= intensity;

	g_color = color;
}