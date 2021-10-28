#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texCoord;
layout(location = 2) in float a_lightValue;

out float v_lightValue;
out vec2 v_texCoord;

uniform mat4 u_modelViewProj;

void main()
{
	gl_Position = u_modelViewProj * vec4(a_position, 1.0f);
	v_lightValue = a_lightValue;
	v_texCoord = a_texCoord;
}