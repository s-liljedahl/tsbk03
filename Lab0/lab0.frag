#version 150

in vec3 exNormal;
in vec3 exSurface;
in vec3 frag_Position;

uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent;
uniform bool isDirectional[4];

out vec4 out_Color;

void main(void)
{
	vec3 i_diffuse;
	vec3 i_ambient;
	vec3 i_spec;

	vec3 i_total;
	vec3 direction;

	vec3 result;

	const float kd = 0.2;
	const float kspec = 0.4;

	// ambient light
	const vec3 lightColor = vec3(0.9, 0.5, 0.9); //ambient light
	float ambientStrength = 0.6; // ljusstyrka
	i_ambient = ambientStrength * lightColor;
	result = i_ambient;

	for(int i = 0; i < 4; i++){

		direction = isDirectional[i] ?
			normalize(lightSourcesDirPosArr[i]) :
			normalize(lightSourcesDirPosArr[i] - frag_Position);

		// diffuse light
		float cos_theta = dot(direction, normalize(exNormal));
		i_diffuse = kd *  lightSourcesColorArr[i] * cos_theta;

		// specular
		vec3 r = (2 * normalize(exNormal) * cos_theta) - direction;
		float cos_phi = max(0, dot(r, normalize(exSurface - frag_Position)));
		i_spec =  kspec * lightSourcesColorArr[i] * pow(cos_phi, specularExponent);

		// resulting light
		i_total = i_spec + i_diffuse;
		result += i_total;
	}

	out_Color=vec4(result,1.0);
}


	// // ambient light
	// const vec3 lightColor = vec3(0.5, 0.9, 1.0); //ambient light
	// float ambientStrength = 0.1; // ljusstyrka
	// i_ambient = ambientStrength * lightColor;
	// total = i_ambient;

	// for(int i = 0; i < 4; i++)
	// {

	// 	vec3 direction;

	// 	if (isDirectional[i]) {
	// 		direction = normalize(lightSourcesDirPosArr[i]);
	// 	} else {
	// 		direction = normalize(lightSourcesDirPosArr[i] - frag_position);
	// 	}


	// 	// diffuse light
	// 	float cos_theta = dot(direction, normalize(exNormal));
	// 	i_diffuse = kd *  lightSourcesColorArr[i] * cos_theta;

	// 	// specular
	// 	vec3 r = (2 * normalize(exNormal) * cos_theta) - direction;

	// 	float cos_phi = max(0, dot(r, normalize(viewDirection - frag_position)));

	// 	i_spec =  kspec * lightSourcesColorArr[i] * pow(cos_phi, specularExponent[i]);

	// 	// resulting light
	// 	result = i_spec + i_diffuse;
	// 	total += result;
	// };

	// out_Color = vec4(total, 1.0);
	// // out_Color = vec4(exNormal, 1.0);