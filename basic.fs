#version 330 core

layout(location = 0) out vec4 f_color;

in float v_lightValue;
in vec2 v_texCoord;
uniform sampler2D u_texture;


uniform vec4 u_color;

void main()
{
    vec4 texColor = texture(u_texture, v_texCoord);
    f_color = texColor * v_lightValue; //* u_color;
}