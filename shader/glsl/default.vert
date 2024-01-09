#version 450
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec3 vColor;
layout (location = 3) in vec2 vTexCoord;

layout (location = 0) out vec3 outColor;
layout (location = 1) out vec3 outNormal;
layout (location = 2) out vec3 outPosition;
layout (location = 3) out vec2 texCoord;

layout (set = 0, binding = 0) uniform CameraBuffer{
	mat4 view;
	mat4 proj;
} cameraBuf;

layout( push_constant ) uniform constants
{
	vec4 data;
	mat4 model;
} PushConstants;

void main(){
   	float distance = 10000.0f;

	vec4 vScreenPos = vec4(vPosition * distance, 1.0f);
	if (gl_VertexIndex == 0){
		vScreenPos = vec4(1.0f, 1.0f, 0.0f, 1.0f);
	}
	
	if (gl_VertexIndex == 1){
		vScreenPos = vec4(1.0f, -1.0f, 0.0f, 1.0f);
	} 
	
	if (gl_VertexIndex == 2){
		vScreenPos = vec4(-1.0f, -1.0f, 0.0f, 1.0f);
	}
	
	if (gl_VertexIndex == 3){
		vScreenPos = vec4(-1.0f, 1.0f, 0.0f, 1.0f);
	}

    gl_Position = vScreenPos;

    outColor = vColor;
	outNormal = vNormal;
	outPosition = vPosition;
    texCoord = vTexCoord;
}
