#version 330 core

layout(location = 0) in vec2 aPos;  
layout(location = 1) in vec2 aTex; 

out vec2 chTex; 
out vec2 vUV;


uniform vec2 uOffset;
uniform vec2 uSize;

void main()
{
    gl_Position = vec4(aPos * uSize + uOffset, 0.0, 1.0);

    vUV = aPos * 0.5 + 0.5;

    chTex = aTex;
}   
