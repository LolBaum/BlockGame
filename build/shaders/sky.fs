#version 330 core //sky
layout (location = 0) out vec4 FragColor;

in vec2 TexCoords;

const vec2 iResolution = vec2(1440.0, 900.0);


void main()
{ 

    FragColor = vec4(TexCoords.xy, 0.0, 1.0);



}