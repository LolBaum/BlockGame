#version 330 core
out vec4 FragColor;

in vec2 v_texCoord;
in float v_lightValue;

uniform sampler2D u_texture;
uniform vec3 u_textColor;

float w = 1920.0;
float h = 1080.0;
float px_w = 1.0/w;
float px_h = 1.0/h;

void main()
{
    vec4 texColor = texture(u_texture, v_texCoord);
    FragColor = texColor;

    FragColor.rgb = u_textColor * v_lightValue;

    //FragColor.a = 0.5;


}
