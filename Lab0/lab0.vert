#version 150

in  vec3  in_Position;
in  vec3  in_Normal;
in  vec2  in_TexCoord;

uniform mat4 projectionMatrix;
uniform mat4 modelToWorldToView;

out vec3 frag_Position;
out vec3 exNormal;
out vec3 exSurface;
out float shade;

void main(void)
{
	// Rotate normal (simple normal matrix, uniform scaling only)
	exNormal = mat3(modelToWorldToView) * in_Normal; 
	
	// Don't include projection here - we only want to go to view coordinates
	exSurface = vec3(modelToWorldToView * vec4(in_Position, 1.0)); 
	
	gl_Position=projectionMatrix*modelToWorldToView*vec4(in_Position, 1.0);
	frag_Position = vec3(gl_Position);

}

