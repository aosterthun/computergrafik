#version 150

in  vec3 pass_Normal;
in  vec3 pass_LightRay;
in  vec3 pass_ViewRay;

in  vec3 pass_ColorAmbient;
in  float pass_Glossiness;
uniform sampler2D pass_TexColor;
in vec2 pass_TexCoord;
flat in  int pass_PlanetType; 

out vec4 out_Color;

void main() {
	
	//Ambient + Diffuse + Specular
	vec3 light_a = vec3(0.2, 0.2, 0.2);
	vec3 light_d = vec3(0.5, 0.5, 0.5);
	vec3 light_s = vec3(1.0, 1.0, 1.0);

	if(pass_PlanetType == 1){
		light_a = vec3(0.8, 0.8, 0.8);
	}
	
	vec3 ambient = pass_ColorAmbient * light_a;
	vec3 diffuse = pass_ColorAmbient * light_d; //ambient == diffuse

	vec3 color = ambient + (diffuse * max(0.0, dot(pass_Normal, pass_LightRay)));

	//Specular
	vec3 h = normalize(pass_ViewRay + pass_LightRay);

	vec3 color_s = vec3(1.0,1.0,1.0) * light_s * pow(max(0.0f,dot(pass_Normal,h)),pass_Glossiness);

	color = color + color_s;

	//overall
	out_Color = vec4(color, 1.0);
}