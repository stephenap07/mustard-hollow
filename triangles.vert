#version 330 core

layout(location=0) in vec4 v_position;
uniform mat4 uni_model;

void main()
{
    gl_Position = uni_model * v_position;
}
