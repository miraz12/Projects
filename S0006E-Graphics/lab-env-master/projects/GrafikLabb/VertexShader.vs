#version 430
uniform mat4 projection;
uniform mat4 model;

const mat4 normalMat = transpose(inverse(model));

out vec2 TexCoord0;
out vec3 normals;
out vec3 vertPos;

layout(location=0) in vec3 pos;
layout(location=1) in vec2 TexCoord;
layout(location=2) in vec3 normal;

void main()
{
	gl_Position = projection * model * vec4(pos, 1.0);
	TexCoord0 = TexCoord;
	/*
	vec4 vertPos4 = model * vec4(pos, 1.0);
	vertPos = vec3(vertPos4) / vertPos4.w;
	normals = vec3(normalMat * vec4(normal, 0.0));*/
	
}