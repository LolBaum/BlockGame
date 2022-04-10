#version 330 core
out vec4 FragColor;
  
in vec2 v_texCoord;

uniform sampler2D u_texture;
uniform vec3 u_textColor;

void main()
{
    vec4 texColor = texture(u_texture, v_texCoord);
    FragColor = texColor;
    if (FragColor.a < 0.5){
        discard;
    }
    FragColor.xyz = u_textColor;
}
