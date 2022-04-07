#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texCoord;
layout(location = 2) in vec4 a_color;

out vec4 v_color;
out vec2 v_texCoord;


uniform mat4 u_modelViewProj;
uniform vec3 u_pos;

void main()
{
	gl_Position = u_modelViewProj * vec4(a_position*1.01 + u_pos, 1.0f);
	v_color = a_color;
	v_texCoord = a_texCoord;
}