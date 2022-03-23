#version 400 core

layout(location = 0) out vec4 accum;
layout(location = 1) out float reveal;

in float v_lightValue;
in vec2 v_texCoord;

uniform sampler2D u_texture;
uniform vec4 u_color;

const vec4 fogcolor = vec4(0.6, 0.7, 1.0, 1.0); // set alpha value lower?
const float fogdensity = 0.0003;



void main()
{	
    vec4 texColor = texture(u_texture, v_texCoord);

    float z = gl_FragCoord.z / gl_FragCoord.w;
    float fog = clamp(exp(-fogdensity * z * z+1)/2, 0.0, 1.0);

	vec4 color = mix(fogcolor, vec4(texColor.xyz * v_lightValue, texColor.w), fog);

    float weight =
        max(min(1.0, max(max(color.r, color.g), color.b) * color.a), color.a) *
        clamp(0.03 / (1e-5 + pow(z / 200, 4.0)), 1e-2, 3e3);

    // blend func: GL_ONE, GL_ONE
    // switch to pre-multiplied alpha and weight
    accum = vec4(color.rgb * color.a, color.a) * weight;

    // blend func: GL_ZERO, GL_ONE_MINUS_SRC_ALPHA
    reveal = color.a;

}