#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_color;
layout (location = 2) in vec2 in_texCoord;

out vec4 color;
out vec2 tex_coords;

void main()
{
	color = vec4(color.xyz, 1.0f);
	tex_coords = in_texCoord;
    gl_Position = vec4(in_position.xyz, 1.0f);
}