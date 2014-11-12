#version 330 core

out vec4 f_color;
uniform vec4 uni_color;

void main()
{
    f_color = uni_color;
}
