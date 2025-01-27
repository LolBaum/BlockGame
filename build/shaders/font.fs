#version 330 core
out vec4 FragColor;
  
in vec2 v_texCoord;

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

    vec4 f0 = texture(u_texture, v_texCoord);
    vec4 f1 = texture(u_texture, v_texCoord + vec2(px_w, px_h));
    vec4 f2 = texture(u_texture, v_texCoord + vec2(-px_w, px_h));
    vec4 f3 = texture(u_texture, v_texCoord + vec2(-px_w, -px_h));
    vec4 f4 = texture(u_texture, v_texCoord + vec2(px_w, -px_h));

    FragColor = (f0*2 + f1*2 + f2*2 + f3*2 + f4*2)/5;

    FragColor.rgb = u_textColor;

    //FragColor.a = 0.5;


}
