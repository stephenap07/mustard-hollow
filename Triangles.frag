#version 330 core

in vec2 v_texcoord;

layout (location = 0) out vec4 color;

uniform int uni_time = 0;
uniform sampler2D tex;
uniform vec4 uni_color;
uniform bool uni_is_textured = false;

void main(void)
{
   float ftime = uni_time / 1000.0f;

   if (uni_is_textured) {
       color = texture(tex, v_texcoord) * uni_color;
   } else {
       color = uni_color;
   }
}
