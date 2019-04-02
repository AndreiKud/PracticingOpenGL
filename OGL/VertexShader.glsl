#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

out vec4 in_color;

void main()
{
	in_color = vec4(aCol.xyz, 1.0f);
    gl_Position = vec4(aPos.xyz, 1.0f);
}