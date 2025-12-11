#version 330 core
in vec2 chTex;          
in vec2 vUV;
out vec4 outCol;

uniform vec4 uColor; 
uniform sampler2D uTexture0;
uniform bool uUseTexture; 

uniform bool uIsLoadingBar;   
uniform float uProgress;     

void main()
{
    if (uIsLoadingBar)
    {
        if (vUV.x <= uProgress)
            outCol = vec4(0.0, 1.0, 0.0, 1.0); 
        else
            outCol = vec4(0.3, 0.3, 0.3, 1.0);
        return;
    }


    if (uUseTexture)
    {
        vec4 texColor = texture(uTexture0, chTex); 
        outCol = texColor * uColor;
    }
    else
    {
        outCol = uColor; 
    }
}
