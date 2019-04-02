#version 330 core

in vec4 color;
in vec2 tex_coords;

out vec4 result_color;

uniform sampler2D uni_texture;

void main()
{
    // result_color = color;
	result_color = texture(uni_texture, tex_coords);
} 