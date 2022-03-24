#version 330 core

layout(location = 0) out vec4 f_color;


in vec4 v_color;
in vec3 v_position;

uniform sampler2D u_texture;


const vec2 iResolution = vec2(1280, 720);
const vec4 fogcolor = vec4(0.6, 0.7, 1.0, 1.0);

vec3 rayDirection(float fieldOfView, vec2 size, vec2 fragCoord) {
    vec2 xy = fragCoord - size / 2.0;
    float z = size.y / tan(radians(fieldOfView) / 2.0);
    return normalize(vec3(xy, -z));
}


void main()
{	

    //vec3 dir = rayDirection(45.0, iResolution.xy, v_position);
    vec3 eye = vec3(0.0, 0.0, 5.0);

	//f_color = v_color;
    f_color = vec4(fogcolor.x, fogcolor.y -0.9 + 1/exp(v_position.y*0.5), fogcolor.z,1.0);//texture(u_texture, v_texCoord);


}