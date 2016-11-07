#version 150

in  vec3 pass_Normal;
in  vec3 pass_LightRay;

in  vec3 pass_ColorAmbient;
in  float pass_Glossiness;

out vec4 out_Color;

void main() {
	
	//Ambient + Diffuse
	vec3 light_a = vec3(0.2, 0.2, 0.2);
	vec3 light_d = vec3(1.0, 1.0, 1.0);

	vec3 ambient = pass_ColorAmbient * light_a;
	vec3 diffuse = pass_ColorAmbient * light_d; //ambient == diffuse

	vec3 color = ambient + (diffuse * max(0.0, dot(pass_Normal, pass_LightRay)));

	//Specular

	//overall
	out_Color = vec4(color, 1.0);
}