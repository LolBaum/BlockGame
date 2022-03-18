#version 330 core

layout(location = 0) out vec4 f_color;


in vec4 v_color;
in vec2 v_texCoord;

uniform sampler2D u_texture;


void main()
{	

	//f_color = v_color;
    f_color = texture(u_texture, v_texCoord);
    if(f_color.a < 0.5)
        discard;

}