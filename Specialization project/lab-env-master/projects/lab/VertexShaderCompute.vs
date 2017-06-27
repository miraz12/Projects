#version 430
uniform mat4 projectionView;
uniform mat4 model;

struct posS
{
	vec4 pxyzw;	// positions
};

struct color
{
	vec4 crgba;	// colors
};

const mat4 normalMat = transpose(inverse(model));

out vec2 TexCoord0;
out vec3 normals;
out vec3 vertPos;

layout(location=0) in vec3 pos;
layout(location=1) in vec2 TexCoord;
layout(location=2) in vec3 normal;

layout( std430, binding=4 ) buffer Pos 
{
	posS  Positions[  ];		// array of structures
};

layout( std430, binding=6 ) buffer Color 
{
	color Colors[  ];		// array of structures
};

void main()
{
	gl_Position =  projectionView * vec4(Positions[gl_InstanceID].pxyzw.xyz, 1.0);
	//gl_PointSize = Positions[gl_InstanceID].pxyz.z;
	TexCoord0 = TexCoord;
	vec4 vertPos4 = model * vec4(pos, 1.0);
	vertPos = vec3(vertPos4) / vertPos4.w;
	normals = Colors[gl_InstanceID].crgba.xyz;
	
}
