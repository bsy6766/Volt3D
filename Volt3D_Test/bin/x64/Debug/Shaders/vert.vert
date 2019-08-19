#version 460

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    // mat4 mvp[3];
    // float test;
} ubo;

// layout(binding = 2) uniform Test {
//     float f;
//     vec2 v2;
//     vec3 v3;
//     vec4 v4;
//     int i;
//     ivec2 i2;
//     ivec3 i3;
//     ivec4 i4;
//     bool b;
//     bvec2 b2;
//     bvec3 b3;
//     bvec4 b4;
//     mat2 m2;
//     mat3 m3;
//     mat4 m4;
//     mat2x3 m23;
//     mat2x4 m24;
//     mat3x2 m32;
//     mat3x4 m34;
//     mat4x2 m42;
//     mat4x3 m43;
// } test;

layout(set = 0, location = 0) in vec3 VERT_POS;
layout(set = 0, location = 1) in vec4 VERT_COLOR;
layout(set = 0, location = 2) in vec2 VERT_TEXCOORD;

layout(location = 0) out vec4 frag_color;
layout(location = 1) out vec2 frag_texcoord;

void main() 
{
    // gl_Position = ubo.proj * ubo.view * ubo.model * vec4(VERT_POS, ubo.test);
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(VERT_POS, 1.0);
    // gl_Position = ubo.mvp[0] * ubo.mvp[1] * ubo.mvp[2] * vec4(VERT_POS, 1.0);
    frag_color = VERT_COLOR;
    frag_texcoord = VERT_TEXCOORD;
}
