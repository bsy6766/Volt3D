#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D TEXTURE_0;
layout(binding = 2) uniform sampler2D TEXTURE_1;

layout(binding = 3) uniform DissolveUBO {
	vec3 color;
	float value;
} ubo;

layout(location = 0) in vec4 frag_color;
layout(location = 1) in vec2 frag_texcoord;

layout(location = 0) out vec4 frag_out;

float normalizeValue(float value, float min, float max)
{
	return (value - min) / (max - min);
}

void main()
{
    // check
	if(ubo.value <= 0.0) discard;
	else if(ubo.value >= 1.0) frag_out = texture(TEXTURE_0, frag_texcoord);
	else
	{
		float d_tex_val = texture(TEXTURE_1, frag_texcoord).r;
		float d_glow_val = clamp(ubo.value - 0.2, 0.0, 1.0);

		// check value
		if(d_glow_val <= d_tex_val && d_tex_val <= ubo.value)
		{
			frag_out.rgb = ubo.color;
			frag_out.a = 1.0 - normalizeValue(d_tex_val, d_glow_val, ubo.value);
		}
		else if(d_tex_val <= d_glow_val)
		{
			frag_out = texture(TEXTURE_0, frag_texcoord);
		}
		else
		{
			// Discard else.
			discard;
		}
	}
}
