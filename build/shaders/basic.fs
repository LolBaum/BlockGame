#version 330 core

layout(location = 0) out vec4 f_color;
//layout(location = 1) out vec4 f_color_1;

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
    float fog = clamp(exp(-fogdensity * z * z+1)/2, 0.0, 1);

	f_color = mix(fogcolor, vec4(texColor.xyz * v_lightValue, texColor.w), fog);
    //f_color_1 = vec4(gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z,1); //vec4(fog,fog,fog,1);
    //f_color = vec4(texColor.xyz * v_lightValue, texColor.w);
    
    
    float test = (gl_FragCoord.z-0.75)*4;
    //f_color = vec4(test,test,test, 1);
    
    
    // this is a tempory solution to make tranparancy work
    if (f_color.a < 0.5)
        discard;

    /* if(f_color.a > 0.9){
        gl_FragDepth = gl_FragCoord.z;
    }
    else{
        gl_FragDepth = 0.9;
    }  */


	//float depth = LinearizeDepth(gl_FragCoord.z)/far;
    //f_color = texColor * v_lightValue * depth; //* u_color;
}