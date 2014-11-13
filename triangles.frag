#version 330 core

in vec2 v_texcoord;

layout (location = 0) out vec4 color;

uniform sampler2D tex;
uniform vec4 uni_color;
uniform bool uni_is_textured = false;

void main(void)
{
   if (uni_is_textured) {
       color = texture(tex, v_texcoord) * uni_color;
   } else {
       color = uni_color;
   }
}
