#version 330 core

layout(location=0) in vec4 position;
layout(location=1) in vec2 texcoord;

out vec2 v_texcoord;

uniform mat4 uni_model;
uniform vec4 uni_tex_xy_uv;

void main()
{
    v_texcoord = uni_tex_xy_uv.xy + (texcoord * uni_tex_xy_uv.zw);
    vec4 new_pos = position;

    gl_Position = uni_model * new_pos;
}
