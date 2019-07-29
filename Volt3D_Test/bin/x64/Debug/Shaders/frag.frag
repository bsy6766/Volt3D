#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D TEXTURE_0;

layout(location = 0) in vec4 frag_color;
layout(location = 1) in vec2 frag_texcoord;

layout(location = 0) out vec4 frag_out;

void main()
{
    frag_out = texture(TEXTURE_0, frag_texcoord) * frag_color;
}
