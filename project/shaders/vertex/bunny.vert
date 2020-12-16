#version 150

in  vec3  inPosition;
in  vec3  inNormal;
in  vec2  inTexCoord;

uniform mat4 projectionMatrix;
uniform mat4 modelToWorldToView;

out vec3 exNormal;
out vec3 exSurface;

void main(void)
{
	// Rotate normal (simple normal matrix, uniform scaling only)
	exNormal = mat3(modelToWorldToView) * inNormal; 
	
	// Don't include projection here - we only want to go to view coordinates
	exSurface = vec3(modelToWorldToView * vec4(inPosition, 1.0)); 
	
	gl_Position=projectionMatrix*modelToWorldToView*vec4(inPosition, 1.0);
}

