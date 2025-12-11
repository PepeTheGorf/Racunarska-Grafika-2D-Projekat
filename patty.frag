#version 330 core
in vec2 chTex;          
out vec4 outCol;

uniform vec4 uColor; 

void main()
{
    outCol = uColor;            
}
