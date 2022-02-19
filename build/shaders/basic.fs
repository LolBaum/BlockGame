#version 330 core

layout(location = 0) out vec4 f_color;

in float v_lightValue;
in vec2 v_texCoord;

uniform sampler2D u_texture;
uniform vec4 u_color;

const vec4 fogcolor = vec4(0.6, 0.7, 1.0, 1.0);
const float fogdensity = 0.0003;


/* float near = 0.1; 
float far  = 100.0; 
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return far -(2.0 * near * far) / (far + near - z * (far - near));	
} */

void main()
{	
    vec4 texColor = texture(u_texture, v_texCoord);

    float z = gl_FragCoord.z / gl_FragCoord.w;
    float fog = clamp(exp(-fogdensity * z * z)+0.1, 0.0, 1);

	f_color = mix(fogcolor, texColor * v_lightValue, fog);
	//float depth = LinearizeDepth(gl_FragCoord.z)/far;
    //f_color = texColor * v_lightValue * depth; //* u_color;
}