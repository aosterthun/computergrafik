#version 150
#extension GL_ARB_explicit_attrib_location : require

uniform sampler2D ColorTex;

uniform uint Luminance;
uniform uint Vertically;
uniform uint Horizontally;
uniform uint Gaussian;

in vec2 pass_TexCoord;

out vec4 out_Color;

void main(void)
{
	float x = pass_TexCoord.x;
	float y = pass_TexCoord.y;
	out_Color = texture(ColorTex, pass_TexCoord);

	if(Vertically == uint(1)){
		//mirror vertical
		x = (1 - x);
		y = y;
		out_Color = texture(ColorTex, vec2(x,y));
	}
	if(Horizontally == uint(1)){
		//mirror horizontally
		x = x;
		y = 1-y;
		out_Color = texture(ColorTex, vec2(x,y));
	}
	if(Gaussian == uint(1)){

		vec2 size = pass_TexCoord / gl_FragCoord.xy;

		vec4 c0 = texture(ColorTex, vec2(x - size.x, y + size.y )) * 1/16;
		vec4 c1 = texture(ColorTex, vec2(x         , y + size.y )) * 1/8;
		vec4 c2 = texture(ColorTex, vec2(x + size.x, y + size.y )) * 1/16;
		vec4 c3 = texture(ColorTex, vec2(x - size.x, y          )) * 1/8;
		vec4 c4 = texture(ColorTex, vec2(x         , y          )) * 1/4;
		vec4 c5 = texture(ColorTex, vec2(x + size.x, y          )) * 1/8;
		vec4 c6 = texture(ColorTex, vec2(x - size.x, y - size.y )) * 1/16;
		vec4 c7 = texture(ColorTex, vec2(x         , y - size.y )) * 1/8;
		vec4 c8 = texture(ColorTex, vec2(x + size.x, y - size.y )) * 1/16;

		out_Color = c0 + c1 + c2 + c3 + c4 + c5 + c6 + c7 + c8;
	}
	if(Luminance == uint(1)){

		float luminance = 0.2126 *out_Color.r + 0.7152*out_Color.r  + 0.0722*out_Color.r;

		out_Color = vec4(luminance,luminance, luminance, 1.0f);
	}

}