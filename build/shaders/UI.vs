#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texCoord;
layout(location = 2) in float a_lightValue;


out vec2 v_texCoord;

uniform mat4 projection;

void main()
{
	gl_Position = projection * vec4(a_position.xy, 0.0, 1.0);
	v_texCoord = a_texCoord;
}
