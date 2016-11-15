#version 150
#extension GL_ARB_explicit_attrib_location : require
// vertex attributes of VAO
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;

//Matrix Uniforms as specified with glUniformMatrix4fv
uniform mat4 NormalMatrix;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

//phong shading
uniform mat4 SunPosition;
uniform vec3  ColorAmbient;
uniform float Glossiness;
uniform int PlanetType;

//variables for fragment shader
out vec3 pass_Normal;
out vec3 pass_LightRay;
out vec3 pass_ColorAmbient;
out vec3 pass_ViewRay;
out float pass_Glossiness;
flat out int pass_PlanetType;

void main(void)
{
	//calculating vertex postion
	gl_Position = (ProjectionMatrix  * ViewMatrix * ModelMatrix) * vec4(in_Position, 1.0);


	vec3 planetPosition = vec3(ModelMatrix * vec4(in_Position, 1.0));
	
	vec3 lightPosition = vec3(0.0, 0.0, 0.0); // vec3(SunPosition * vec4(1.0, 1.0, 1.0, 1.0));

	//passing values for phong shading
	pass_Normal   =  normalize((NormalMatrix * vec4(in_Normal, 0.0)).xyz);
	pass_LightRay =  normalize(lightPosition - planetPosition.xyz);
	pass_ViewRay  =  normalize(- planetPosition.xyz); //normalize(pass_LightRay-planetPosition.xyz);

	pass_ColorAmbient = ColorAmbient;
	pass_Glossiness   = Glossiness;
	pass_PlanetType   = PlanetType;
}