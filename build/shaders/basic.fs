#version 330 core

layout(location = 0) out vec4 f_color;

in float v_lightValue;
in vec2 v_texCoord;
uniform sampler2D u_texture;


uniform vec4 u_color;


float near = 0.1; 
float far  = 100.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return far -(2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{	
    vec4 texColor = texture(u_texture, v_texCoord);
	f_color = texColor * v_lightValue;
	//float depth = LinearizeDepth(gl_FragCoord.z)/far;
    //f_color = texColor * v_lightValue * depth; //* u_color;
}