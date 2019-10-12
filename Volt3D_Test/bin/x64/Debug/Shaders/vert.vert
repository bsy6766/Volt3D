#version 460

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;


layout(set = 0, location = 0) in vec3 VERT_POS;
layout(set = 0, location = 1) in vec4 VERT_COLOR;
layout(set = 0, location = 2) in vec2 VERT_TEXCOORD;

layout(location = 0) out vec4 frag_color;
layout(location = 1) out vec2 frag_texcoord;

void main() 
{
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(VERT_POS, 1.0);
    frag_color = VERT_COLOR;
    frag_texcoord = VERT_TEXCOORD;
}
