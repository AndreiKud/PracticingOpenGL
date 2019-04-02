#version 330

in vec4 in_color;
out vec4 FragColor;

uniform vec4 uniColor;

void main()
{
    FragColor = uniColor;
} 