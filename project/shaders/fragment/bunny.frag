#version 150

in vec3 exNormalG; // Phong
in vec3 exSurfaceG;
in vec3 frag_Position;

uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform bool isDirectional[4];
uniform float index;

out vec4 outColor;


void main(void)
{
	vec3 i_diffuse, i_ambient, i_spec, i_total;
	vec3 direction, result;

	const float specularExponent = 200.0;
	const float kd = 0.2;
	const float kspec = 0.4;

	// ambient light
	const vec3 lightColor = vec3(0.8, 0.8, 0.8); //ambient light
	float ambientStrength = 0.6; // ljusstyrka
	i_ambient = ambientStrength * lightColor;
	result = i_ambient;

	for(int i = 0; i < 4; i++){

		direction = isDirectional[i] ?
			normalize(lightSourcesDirPosArr[i]) :
			normalize(lightSourcesDirPosArr[i] - frag_Position);

		// diffuse light
		float cos_theta = dot(direction, normalize(exNormalG));
		i_diffuse = kd *  lightSourcesColorArr[i] * cos_theta;

		// specular
		vec3 r = (2 * normalize(exNormalG) * cos_theta) - direction;
		float cos_phi = max(0, dot(r, normalize(exSurfaceG - frag_Position)));
		i_spec =  kspec * lightSourcesColorArr[i] * pow(cos_phi, specularExponent);

		// resulting light
		i_total = i_spec + i_diffuse;
		result += i_total;
	}

	outColor=vec4(result,1.0);
}
