#version 460

layout(location = 0) in vec3 VERT_POS;
layout(location = 1) in vec3 VERT_COLOR;

layout(location = 0) out vec3 frag_color;

void main() 
{
    gl_Position = vec4(VERT_POS, 1.0);
    frag_color = VERT_COLOR;
}
